#include <cassert>
#include <set>
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

            if (check_if_valid_placement(current_cursor_.piece, current_cursor_.x,
                current_cursor_.y, half_squares_placed_)) {

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

bool Game::check_if_valid_placement(std::shared_ptr<Piece> piece, int x, int y, int half_squares_placed)
    const {
    bool is_enough_half_squares_left = check_if_sufficient_half_squares_left(piece, half_squares_placed);

    if (!is_enough_half_squares_left) {
        return false;
    }

    bool is_space_in_slot = check_if_space_in_board_slot(piece, x, y);

    if (!is_space_in_slot) {
        return false;
    }

    bool is_connected = check_if_connected_to_existing_pieces(piece, x, y);

    if (!is_connected) {
        return false;
    }

    return true;
}

bool Game::check_if_sufficient_half_squares_left(std::shared_ptr<Piece> piece, int half_squares_placed) const {
    if (half_squares_placed == 1 && piece->get_piece_type() == Piece::piece_type::square) {
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

void shift_points_inplace(std::vector<Piece::Point>& points, int x, int y) {
    Piece::Point x_y_shift = {x * 2, y * -2};

    for (Piece::Point& p : points) {
        p += x_y_shift;
    }
}

bool Game::check_if_connected_to_existing_pieces(std::shared_ptr<Piece> piece, int x, int y) const {
    
    const Board::board_pointer board = board_.get_board();
    std::vector<Piece::Point> piece_points = piece->get_points();
    const int piece_owner_id = piece->get_owner_id();

    // Shift piece's coordinates over based on x and y
    shift_points_inplace(piece_points, x, y);

    // Get area to check in
    int x_start = x - 1;
    int y_start = y - 1;
    int x_end = x + 2; // Add two to be one past the slot to check
    int y_end = y + 2;

    // Enforce bounds of board
    if (x_start < 0) {
        x_start = 0;
    }

    if (x_end > Board::board_size) {
        x_end = Board::board_size;
    }
    
    if (y_start < 0) {
        y_start = 0;
    }

    if (y_end > Board::board_size) {
        y_end = Board::board_size;
    }

    // Loop through all the pieces in the given area
    for (int i = x_start; i < x_end; ++i) {
        for (int j = y_start; j < y_end; ++j) {
            Board::board_slot slot = board[j][i];
       
            if (slot.first && slot.first->get_owner_id() == piece_owner_id) {
                // Need to offset the pieces based on their location
                std::vector<Piece::Point> points = slot.first->get_points();
                shift_points_inplace(points, i, j);
                
                if (compare_points(points, piece_points)) {
                    return true;
                }
            }
            
            if (slot.second && slot.second->get_owner_id() == piece_owner_id) {
                std::vector<Piece::Point> points = slot.second->get_points();
                shift_points_inplace(points, i, j);
                
                if (compare_points(points, piece_points)) {
                    return true;
                }
            }
        }
    }


    return false;
}

// Checks if any of the points in one vector match the other
bool Game::compare_points(std::vector<Piece::Point> points1, std::vector<Piece::Point> points2) const {
    for (auto& p1 : points1) {
        for (auto& p2 : points2) {
            if (p1 == p2) {
                return true;
            }
        }
    }

    return false;
}

bool Game::check_if_game_is_finished(Board& final_board) {
    // Create a board for each actor based on the current board
    std::vector<Board> boards;

    for (int i = 0; i < num_actors_; ++i) {
        Board new_board = board_;
        boards.push_back(new_board);
    }

    // Treat it as if the actor is the only actor and places until the board is full
    for (int i = 0; i < num_actors_; ++i) {
        simulate_filling_placements(boards[i], i);
    }
    
    // Add these pieces to a new board and check for overlap.
    // If there is overlap it means that the game can't be done since actors can 
    // place in the same spot.

    Board::board_pointer final_board_arr = final_board.get_board();

    for (Board& board : boards) {
        Board::board_pointer board_arr = board.get_board();
        for (int i = 0; i < Board::board_size; ++i) {
            for (int j = 0; j < Board::board_size; ++j) {
                 
                Board::board_slot slot = board_arr[i][j];
                Board::board_slot final_slot = final_board_arr[i][j];

                // Check if we have anything to place
                if (slot.first) {
               
                    // Full slot
                    // Check for a single square that matches
                    std::cout << "here1" << "\n";

                    if (final_slot.first && 
                        final_slot.first->get_piece_type() == Piece::piece_type::square) {
                        
                        bool first_slot_same = *slot.first.get() == *final_slot.first.get();
                        if (!first_slot_same) {
                            return false;
                        }

                        continue;
                    }
                    std::cout << "here2" << "\n";
                    
                    // Empty slots
                    if (!final_slot.first) {
                        // We can use slot.first here since we must have something to place
                        final_board.place_piece(slot.first, j, i);

                        // Check if we have a second piece to place
                        if (slot.second) {
                            final_board.place_piece(slot.second, j, i);
                        }

                        continue;
                    }
                    std::cout << "here3" << "\n";

                    // Full Slot
                    // Must have the same pieces in slot
                    // otherwise it's an overlap
                    if (final_slot.first && final_slot.second) {
                        bool first_slot_same = *slot.first.get() == *final_slot.first.get();
                        if (!first_slot_same) {
                            return false;
                        } else {
                            if (slot.second) {
                                bool second_slot_same = *slot.second.get() ==
                                    *final_slot.second.get();
                                if (!second_slot_same) {
                                    return false;
                                }
                            }
                            continue;
                        }
                    }

                    // Partially filled slot
                    // Check first slot match since we know there must be something there
                    std::cout << "here?" << "\n";
                    if (!final_slot.first) {
                        std::cout << "???" << "\n";
                    }

                    bool first_slot_same = *slot.first.get() == *final_slot.first.get();
                    if (first_slot_same) {
                        // If the first slot is the same, then the second piece must fit
                        std::cout << "W" << "\n";
                        if (slot.second) {
                            final_board.place_piece(slot.second, j, i);
                        }
                    } else {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool Game::fill_slot(Board& board, const Board::board_slot& slot, int id, int i, int j) {

    // Check if there is space in the slot
    if (!slot.first || !slot.second) {

        // Try to place each type of piece in the slot
        // Start with a square since that fills an empty slot the fastest
        // And also ensures we only have to place one piece if there is space
        std::shared_ptr<Piece> piece = std::make_shared<Square>(id);

        // Square
        if (check_if_valid_placement(piece, i, j, 0)) {
            board.place_piece(piece, i, j);
            return true;
        }

        // Triangles
        piece = std::make_shared<Triangle>(id);
        for (int rotations = 0; rotations < 4; ++rotations) {
            if (check_if_valid_placement(piece, j, i, 0)) {
                board.place_piece(piece, j, i);
                return true;
            }
            piece->rotate();
        }

        // Rectangles
        piece = std::make_shared<Rectangle>(id);
        for (int rotations = 0; rotations < 4; ++rotations) {
            if (check_if_valid_placement(piece, j, i, 0)) {
                board.place_piece(piece, j, i);
                return true;
            }
            piece->rotate();
        }
    }

    return false;
}

void Game::simulate_filling_placements(Board& board, int id) {
   
    // Create queue of where we can possibly place
    std::set<board_pos> potential_positions;
    std::set<board_pos> considered_positions;

    // Get possible placement positions using current board locations
    for (int i = 0; i < Board::board_size; ++i) {
        for (int j = 0; j < Board::board_size; ++j) {
            Board::board_slot slot = board_.get_board()[i][j];
            
            // Get area to check within
            int i_start = i - 1;
            int j_start = j - 1;
            int i_end = i + 2; // Add two to be one past the slot to check
            int j_end = j + 2;

            // Enforce bounds of board
            if (i_start < 0) {
                i_start = 0;
            }

            if (i_end > Board::board_size) {
                i_end = Board::board_size;
            }
            
            if (j_start < 0) {
                j_start = 0;
            }

            if (j_end > Board::board_size) {
                j_end = Board::board_size;
            }
            
            bool first = slot.first && slot.first->get_owner_id() == id;
            bool second = slot.second && slot.second->get_owner_id() == id;
            // Add potential positions around a piece
            if (first || second) {
                for (int h = i_start; h < i_end; ++h) {
                    for (int w = j_start; w < j_end; ++w) {
                        board_pos new_pos = {h, w};
                        std::cout << "Adding pos: " << h << " " << w << "\n";
                        if (auto search = considered_positions.find(new_pos);
                            search == considered_positions.end()) {
                            potential_positions.insert(new_pos);
                            considered_positions.insert(new_pos);
                        }
                    }
                }
            }
        }
    }

    // Go through all positions
    while (!potential_positions.empty()) {
        board_pos pos = potential_positions.extract(potential_positions.begin()).value();

        std::cout << "Checking position: " << pos.i << " " << pos.j << "\n";

        bool filled_slot = fill_slot(board, board_.get_board()[pos.i][pos.j], id, pos.i, pos.j);

        // Add all nearby positions
        if (filled_slot) {

            const int i = pos.i;
            const int j = pos.j;

            // Get area to check within
            int i_start = i - 1;
            int j_start = j - 1;
            int i_end = i + 2; // Add two to be one past the slot to check
            int j_end = j + 2;

            // Enforce bounds of board
            if (i_start < 0) {
                i_start = 0;
            }

            if (i_end > Board::board_size) {
                i_end = Board::board_size;
            }
            
            if (j_start < 0) {
                j_start = 0;
            }

            if (j_end > Board::board_size) {
                j_end = Board::board_size;
            }
            
            // Add potential positions around a piece
            for (int h = i_start; h < i_end; ++h) {
                for (int w = j_start; w < j_end; ++w) {
                    board_pos new_pos = {h, w};
                    if (auto search = considered_positions.find(new_pos);
                        search == considered_positions.end()) {
                        potential_positions.insert(new_pos);
                        considered_positions.insert(new_pos);
                    }
                }
            }
        }
    }

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
