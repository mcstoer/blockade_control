#include <iostream>
#include <cassert>

#include "components/piece.hpp"
#include "components/triangle.hpp"


// Creates triangle pointing towards top left corner
// Points are in -1,1 space and in clockwise order
Triangle::Triangle(int owner_id) : Piece(owner_id) {
    points_.push_back(Point{-1, -1});
    points_.push_back(Point{-1, 1});
    points_.push_back(Point{1, 1});
    rotation_ = 0;
    piece_type_ = piece_type::triangle;

}

Triangle::~Triangle() {};

void Triangle::rotate() {

    switch(rotation_) {
        case 0:
            rotation_ = 90;
            points_[0] = {-1, 1};
            points_[1] = {1, 1};
            points_[2] = {1, -1};
            break;

        case 90: 
            rotation_ = 180;
            points_[0] = {1, 1};
            points_[1] = {1, -1};
            points_[2] = {-1, -1};
            break;

        case 180:
            rotation_ = 270;
            points_[0] = {1, -1};
            points_[1] = {-1, -1};
            points_[2] = {-1, 1};
            break;

        case 270: 
            rotation_ = 0;
            points_[0] = {-1, -1};
            points_[1] = {-1, 1};
            points_[2] = {1, 1};
            break;

        // Should never hit default
        default:
            assert(false);
    }
}
