#include <cassert>

#include "components/piece.hpp"
#include "components/rectangle.hpp"


// Creates rectangle with top left corner of the rectangle in top left corner
// of a square. The rectangle takes up half of a square.
// The origin (0,0) of the space is considered to be at the center of 
// a square. The points are in clockwise order starting at the middle left.
Rectangle::Rectangle(int owner_id) : Piece(owner_id) {
    points_.push_back(Point{-1, 0});
    points_.push_back(Point{-1, 1});
    points_.push_back(Point{1, 1});
    points_.push_back(Point{1, 0});
    rotation_ = 0;
    piece_type_ = piece_type::rectangle;

}

Rectangle::~Rectangle() {};

std::shared_ptr<Piece> Rectangle::clone() const {
    return std::make_shared<Rectangle>(*this);
}
