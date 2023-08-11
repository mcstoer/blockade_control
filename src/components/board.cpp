#include <memory>
#include <cmath>

#include <components/board.hpp>
#include <components/piece.hpp>
#include <components/square.hpp>

Board::Board() {
    place_initial_squares();    
}

const Board::board_pointer Board::get_board() const {
    return board_.begin();
}

bool Board::place_piece(std::shared_ptr<Piece> piece, int x, int y) {

    bool success = false;

    // Check if there is any space to place a piece
    if (board_[y][x].first && board_[y][x].second) {
        success = false;
    } else {
        // Try to place in the first slot first
        if (!board_[y][x].first) {
            board_[y][x].first = piece;
        } else {
            board_[y][x].second = piece;
        }

        success = true;
    }

    return success;
}

void Board::clear() {

    // Reset all the smart pointers
    for (auto rowIter = board_.begin(); rowIter != board_.end(); ++rowIter) {
        for (auto colIter = rowIter->begin(); colIter != rowIter->end(); ++colIter) {
            colIter->first.reset();
            colIter->second.reset();
        }
    }

    // Add the initial squares back
    place_initial_squares();
}

float Board::get_score(int id) const {
    float score = 0;

    for (auto rowIter = board_.begin(); rowIter != board_.end(); ++rowIter) {
        for (auto colIter = rowIter->begin(); colIter != rowIter->end(); ++colIter) {
           
            if (colIter->first && colIter->first->get_owner_id() == id) {
                if (colIter->first->get_piece_type() == Piece::piece_type::square) {
                    score += 1.0f;
                } else {
                    score += 0.5f;
                }
            }
            
            if (colIter->second && colIter->second->get_owner_id() == id) {
                if (colIter->second->get_piece_type() == Piece::piece_type::square) {
                    score += 1.0f;
                } else {
                    score += 0.5f;
                }
            }
        }
    }

    // The result should always without decimals
    return score;
}

void Board::place_initial_squares() {

    const int player_one_id = 0;
    const int player_two_id = 1;
    
    // Put squares in the corners for each player
    board_[board_size - 1][0] = std::pair{
                                std::make_unique<Square>(Square(player_one_id)),
                                std::shared_ptr<Piece>()
                                };
    board_[0][board_size - 1] = std::pair{
                                std::make_unique<Square>(Square(player_two_id)),
                                std::shared_ptr<Piece>()
                                };
}


