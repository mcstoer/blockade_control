#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <algorithm>

#include "components/board.hpp"
#include "components/piece.hpp"
#include "components/triangle.hpp"
#include "components/square.hpp"
#include "components/rectangle.hpp"


TEST_CASE("Triangle Construction", "[triangle, construction]") {
    int id = 5;
    Triangle t = Triangle(id);

    CHECK(t.get_owner_id() == id);
    CHECK(t.get_piece_type() == Piece::piece_type::triangle);

    std::vector<Piece::Point> points = t.get_points();
    std::vector<Piece::Point> expected_points = {Piece::Point{-1, -1},
        Piece::Point{-1, 1}, Piece::Point{1, 1}};

    CHECK(points == expected_points);
}
