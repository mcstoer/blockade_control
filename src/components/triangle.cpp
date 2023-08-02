#include <iostream>

#include "components/piece.hpp"
#include "components/triangle.hpp"


// Creates triangle pointing towards top left corner
// Points are in -1,1 space and in clockwise order
Triangle::Triangle() : Piece() {
    points_.push_back(Point{-1, -1});
    points_.push_back(Point{-1, 1});
    points_.push_back(Point{1, 1});
    rotation_ = 0;
    piece_type_ = piece_type::triangle;

}

Triangle::~Triangle() {};

void Triangle::rotate() {

}
