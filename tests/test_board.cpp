#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <memory>

#include "components/board.hpp"
#include "components/piece.hpp"
#include "components/rectangle.hpp"


TEST_CASE("place", "[place]") {

    Board board; 

    std::shared_ptr<Piece> p = std::make_shared<Rectangle>(0);
    std::shared_ptr<Piece> p2 = std::make_shared<Rectangle>(1);
    std::shared_ptr<Piece> p3 = std::make_shared<Rectangle>(1);

    int x = 5;
    int y = 1;

    REQUIRE(board.place_piece(p, x, y));
    p2->rotate();
    p2->rotate();
    REQUIRE(board.place_piece(p2, x, y));
    REQUIRE_FALSE(board.place_piece(p3, x, y));


    auto ptr = board.get_board();

    REQUIRE(ptr[y][x].first->get_points() == p->get_points());
    REQUIRE(ptr[y][x].second->get_points() == p2->get_points());
}

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


