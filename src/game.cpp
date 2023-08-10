#include <cassert>
#include <thread>
#include <chrono>
#include <iostream>

#include "game.hpp"
#include "actors/player.hpp"
#include "components/piece.hpp"
#include "components/triangle.hpp"
#include "components/square.hpp"
#include "components/rectangle.hpp"

Game::Game(int blocks, int players, InputHandler* input_handler) {
    blocks_ = blocks;
    num_actors_ = players;
    input_handler_ = input_handler;

    // Set starting turn
    current_actor_turn_ = 0;
    half_squares_placed_ = 0;

    reset_cursor(current_actor_turn_);

    // Create players
    for (int i = 0; i < players; ++i) {
        players_.push_back(Player(i, input_handler_));
    }
}

void Game::progress_turn() {
    Player current_player = players_[current_actor_turn_];

    Action player_action = current_player.do_action();

    // Check if action is valid

    // Apply action's effect
    switch (player_action) {
        case Action::NONE:
            return; // Try to avoid sleeping to act faster after sleep.

        case Action::UP:
            if (current_cursor_.y > 0) {
                --current_cursor_.y;
            }
            break;

        case Action::DOWN: 
            if (current_cursor_.y < Board::board_size - 1) {
                ++current_cursor_.y;
            }
            break;

        case Action::LEFT:
            if (current_cursor_.x > 0) {
                --current_cursor_.x;
            }
            break;

        case Action::RIGHT: 
            if (current_cursor_.x < Board::board_size - 1) {
                ++current_cursor_.x;
            }
            break;

        case Action::ROTATE: 
            current_cursor_.piece->rotate();
            break;

        case Action::TOGGLE: 
            toggle_cursor_piece();
            break;

        case Action::PLACE:

            if (check_if_valid_placement(current_cursor_.piece, current_cursor_.x, current_cursor_.y)) {

                board_.place_piece(current_cursor_.piece, current_cursor_.x, current_cursor_.y);

                // Update the number of half squares placed by the actor
                if (current_cursor_.piece->get_piece_type() == Piece::piece_type::square) {
                    half_squares_placed_ = 2;
                } else {
                    ++half_squares_placed_;
                }
                    
                if (half_squares_placed_ == 2) {
                    half_squares_placed_ = 0;
                    current_actor_turn_ = get_next_actor();
                }
                
                // Reset after any potential changes to the actor id
                reset_cursor(current_actor_turn_);
                
                // Add additional sleep to prevent double placing
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            break;

        default: 
            assert(false);
    }

    // Sleep to give the player time to react to the piece movements
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
}

bool Game::check_if_valid_placement(std::shared_ptr<Piece> piece, int x, int y) const {
    bool is_enough_half_squares_left = check_if_sufficient_half_squares_left(piece);

    if (!is_enough_half_squares_left) {
        return false;
    }

    bool is_space_in_slot = check_if_space_in_board_slot(piece, x, y);

    if (!is_space_in_slot) {
        return false;
    }

    return true;
}

bool Game::check_if_sufficient_half_squares_left(std::shared_ptr<Piece> piece) const {
    if (half_squares_placed_ == 1 && piece->get_piece_type() == Piece::piece_type::square) {
        return false;
    }
    
    return true;
}

bool Game::check_if_space_in_board_slot(std::shared_ptr<Piece> piece, int x, int y) const {
    const Board::board_pointer board = board_.get_board();
    Board::board_slot slot = board[y][x]; // Switch these since board is 2d array

    // Space in both slots
    if (!slot.first && !slot.second) {
        return true;

    // Space in the second slot
    } else if (slot.first && !slot.second) {
        
        // A square can be the only thing in a slot
        if (piece->get_piece_type() != Piece::piece_type::square 
            || slot.first->get_piece_type() != Piece::piece_type::square) {
            
            // Both pieces in a slot need to match type
            if (slot.first->get_piece_type() == piece->get_piece_type()) {
                
                // The first piece and second piece need to be different by a rotation of 180 degrees
                std::shared_ptr<Piece> piece_copy = piece->clone();

                // Rotate 180
                piece_copy->rotate();
                piece_copy->rotate();

                if (*slot.first.get() == *piece_copy.get()) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void Game::reset_cursor(int id) {
    current_cursor_.x = 0;
    current_cursor_.y = 0;
    current_cursor_.player_id = id;
    current_cursor_.piece = std::make_shared<Triangle>(id);
}

void Game::toggle_cursor_piece() {
    
    int id = current_cursor_.player_id;

    switch (current_cursor_.piece->get_piece_type()) {
        case Piece::piece_type::triangle: 
            current_cursor_.piece = std::make_shared<Rectangle>(id);
            break;

        case Piece::piece_type::rectangle: 
            current_cursor_.piece = std::make_shared<Square>(id);
            break;

        case Piece::piece_type::square: 
            current_cursor_.piece = std::make_shared<Triangle>(id);
            break;

        default: 
            assert(false);
    }
}

int Game::get_next_actor() {
    return (current_actor_turn_ + 1) % num_actors_;
}

Board Game::get_board() const {
    return board_;
}

Game::Cursor Game::get_cursor() const {
    return current_cursor_;
}
