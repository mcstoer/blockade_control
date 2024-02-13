#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <memory>

#include "components/board.hpp"
#include "components/piece.hpp"
#include "components/rectangle.hpp"

// Tests the placement of valid and invalid placements. Also checks if
// placing pieces on the board does not modify the points.
TEST_CASE("place", "[place]") {

    Board board; 

    std::shared_ptr<Piece> p = std::make_shared<Rectangle>(0);
    std::shared_ptr<Piece> p2 = std::make_shared<Rectangle>(1);
    std::shared_ptr<Piece> p3 = std::make_shared<Rectangle>(1);

    int x = 5;
    int y = 1;

    // First placement on a empty board is valid.
    REQUIRE(board.place_piece(p, x, y));
    p2->rotate();
    p2->rotate();

    // Second placement by different owner in the same square but
    // 180 degrees rotated is valid.
    REQUIRE(board.place_piece(p2, x, y));

    // Third placement on a filled square should be invalid.
    REQUIRE_FALSE(board.place_piece(p3, x, y));


    auto ptr = board.get_board();

    // The points should not get modified by placements.
    REQUIRE(ptr[y][x].first->get_points() == p->get_points());
    REQUIRE(ptr[y][x].second->get_points() == p2->get_points());
}

// Tests clearing a board of all pieces.
TEST_CASE("clear", "[clear]") {

    Board board; 

    std::shared_ptr<Piece> p = std::make_shared<Rectangle>(0);

    int x = 5;
    int y = 1;

    board.place_piece(p, x, y);

    board.clear();

    auto ptr = board.get_board();

    REQUIRE(!ptr[y][x].first);
}


