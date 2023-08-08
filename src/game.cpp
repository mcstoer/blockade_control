#include <cassert>
#include <thread>
#include <chrono>

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
            board_.place_piece(current_cursor_.piece, current_cursor_.x, current_cursor_.y);
            current_actor_turn_ = get_next_actor();
            reset_cursor(current_actor_turn_);
            
            // Add additional sleep to prevent double placing
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            break;

        default: 
            assert(false);
    }

    // Sleep to give the player time to react to the piece movements
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
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
