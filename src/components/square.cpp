#include <cassert>

#include "components/piece.hpp"
#include "components/square.hpp"

// Creates a square with no initial rotation, so the sides are 
// parallel to the board.
// The origin (0,0) of the space is considered to be at the center of a 
// square. The points are in clockwise order starting from bottom left
Square::Square(int owner_id) : Piece(owner_id) {
    points_.push_back(Point{-1, -1});
    points_.push_back(Point{-1, 1});
    points_.push_back(Point{1, 1});
    points_.push_back(Point{1, -1});
    rotation_ = 0;
    piece_type_ = piece_type::square;
}

Square::~Square() {};

// Rotating a square only changes the order of the points,
// so for simplicity the points aren't modified.
void Square::rotate() {

    switch(rotation_) {
        case 0:
            rotation_ = 90;
            break;

        case 90: 
            rotation_ = 180;
            break;

        case 180:
            rotation_ = 270;
            break;

        case 270: 
            rotation_ = 0;
            break;

        // Should never hit default
        default:
            assert(false);
    }
}

std::shared_ptr<Piece> Square::clone() const {
    return std::make_shared<Square>(*this);
}
