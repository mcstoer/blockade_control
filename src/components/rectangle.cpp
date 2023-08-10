#include <cassert>

#include "components/piece.hpp"
#include "components/rectangle.hpp"


// Creates rectangle with top left corner of rectangle in top left corner
// Rectangle is half of a square
// Points are in -1,1 space and in clockwise order
Rectangle::Rectangle(int owner_id) : Piece(owner_id) {
    points_.push_back(Point{-1, 0});
    points_.push_back(Point{-1, 1});
    points_.push_back(Point{1, 1});
    points_.push_back(Point{1, 0});
    rotation_ = 0;
    piece_type_ = piece_type::rectangle;

}

Rectangle::~Rectangle() {};

void Rectangle::rotate() {

    switch(rotation_) {
        case 0:
            rotation_ = 90;
            points_[0] = {0, 1};
            points_[1] = {1, 1};
            points_[2] = {1, -1};
            points_[3] = {0, -1};
            break;

        case 90: 
            rotation_ = 180;
            points_[0] = {1, 0};
            points_[1] = {1, -1};
            points_[2] = {-1, -1};
            points_[3] = {-1, 0};
            break;

        case 180:
            rotation_ = 270;
            points_[0] = {0, -1};
            points_[1] = {-1, -1};
            points_[2] = {-1, 1};
            points_[3] = {0, 1};
            break;

        case 270: 
            rotation_ = 0;
            points_[0] = {-1, 0};
            points_[1] = {-1, 1};
            points_[2] = {1, 1};
            points_[3] = {1, 0};
            break;

        // Should never hit default
        default:
            assert(false);
    }
}

std::shared_ptr<Piece> Rectangle::clone() const {
    return std::make_shared<Rectangle>(*this);
}
