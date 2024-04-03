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

std::shared_ptr<Piece> Triangle::clone() const {
    return std::make_shared<Triangle>(*this);
}
