#include "components/piece.hpp"

Piece::Piece(int owner_id) {
    owner_id_ = owner_id;
}

std::vector<Piece::Point> Piece::get_points() const {
    return points_; 
}

Piece::piece_type Piece::get_piece_type() const {
    return piece_type_;
}

int Piece::get_owner_id() const {
    return owner_id_;
}
