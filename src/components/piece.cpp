#include "components/piece.hpp"

std::vector<Piece::Point> Piece::get_points() const {
    return points_; 
}

Piece::piece_type Piece::get_piece_type() const {
    return piece_type_;
}
