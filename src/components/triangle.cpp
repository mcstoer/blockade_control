#include <iostream>
#include <cassert>

#include "components/piece.hpp"
#include "components/triangle.hpp"


// The triangles takes up half a square.
// The origin (0,0) of the space is considered to be at the center of 
// a square. The points are in clockwise order starting from the bottom right.
Triangle::Triangle(int owner_id) : Piece(owner_id) {
    points_.push_back(Point{-1, -1});
    points_.push_back(Point{-1, 1});
    points_.push_back(Point{1, 1});
    rotation_ = 0;
    piece_type_ = piece_type::triangle;

}

Triangle::~Triangle() {};

// Rotating the triangle is considered to be moving between 4 states for 
// simplicity.
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

std::shared_ptr<Piece> Triangle::clone() const {
    return std::make_shared<Triangle>(*this);
}
