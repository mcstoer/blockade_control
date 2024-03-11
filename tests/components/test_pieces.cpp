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

TEST_CASE("Triangle Rotation", "[triangle, rotation]") {
    Triangle t = Triangle(0);

    // 90 degrees clockwise
    t.rotate();
    std::vector<Piece::Point> points = t.get_points();
    std::vector<Piece::Point> expected_points = {Piece::Point{-1, 1},
        Piece::Point{1, 1}, Piece::Point{1, -1}};
    CHECK(points == expected_points);

    // 180 degrees clockwise
    t.rotate();
    points = t.get_points();
    expected_points = {Piece::Point{1, 1}, Piece::Point{1, -1}, Piece::Point{-1, -1}};
    CHECK(points == expected_points);

    // 270 degrees clockwise
    t.rotate();
    points = t.get_points();
    expected_points = {Piece::Point{1, -1}, Piece::Point{-1, -1}, Piece::Point{-1, 1}};
    CHECK(points == expected_points);

    // 0 (360) degrees clocksise
    t.rotate();
    points = t.get_points();
    expected_points = {Piece::Point{-1, -1}, Piece::Point{-1, 1}, Piece::Point{1, 1}};
    CHECK(points == expected_points);
}

TEST_CASE("Triangle Cloning", "[triangle, clone]") {
    Triangle t = Triangle(0);
    std::shared_ptr<Piece> t_clone = t.clone();
    REQUIRE(*t_clone.get() == t);
}

TEST_CASE("Rectangle Construction", "[rectangle, construction]") {
    int id = 10;
    Rectangle r = Rectangle(id);

    CHECK(r.get_owner_id() == id);
    CHECK(r.get_piece_type() == Piece::piece_type::rectangle);

    std::vector<Piece::Point> points = r.get_points();
    std::vector<Piece::Point> expected_points = {Piece::Point{-1, 0}, Piece::Point{-1, 1},
        Piece::Point{1, 1}, Piece::Point{1, 0}};

    CHECK(points == expected_points);
}

TEST_CASE("Rectangle Rotation", "[rectangle, rotation]") {
    Rectangle r = Rectangle(0);

    // 90 degrees clockwise
    r.rotate();
    std::vector<Piece::Point> points = r.get_points();
    std::vector<Piece::Point> expected_points = {Piece::Point{0, 1}, Piece::Point{1, 1},
        Piece::Point{1, -1}, Piece::Point{0, -1}};
    CHECK(points == expected_points);

    // 180 degrees clockwise
    r.rotate();
    points = r.get_points();
    expected_points = {Piece::Point{1, 0}, Piece::Point{1, -1}, Piece::Point{-1, -1},
        Piece::Point{-1, 0}};
    CHECK(points == expected_points);

    // 270 degrees clockwise
    r.rotate();
    points = r.get_points();
    expected_points = {Piece::Point{0, -1}, Piece::Point{-1, -1}, Piece::Point{-1, 1},
        Piece::Point{0, 1}};
    CHECK(points == expected_points);

    // 0 (360) degrees clocksise
    r.rotate();
    points = r.get_points();
    expected_points = {Piece::Point{-1, 0}, Piece::Point{-1, 1}, Piece::Point{1, 1},
        Piece::Point{1, 0}};
    CHECK(points == expected_points);
}

TEST_CASE("Rectangle Cloning", "[rectangle, clone]") {
    Rectangle r = Rectangle(0);
    std::shared_ptr<Piece> r_clone = r.clone();
    REQUIRE(*r_clone.get() == r);
}

TEST_CASE("Square Construction", "[square, construction]") {
    int id = 10;
    Square s = Square(id);

    CHECK(s.get_owner_id() == id);
    CHECK(s.get_piece_type() == Piece::piece_type::square);

    std::vector<Piece::Point> points = s.get_points();
    std::vector<Piece::Point> expected_points = {Piece::Point{-1, -1}, Piece::Point{-1, 1},
        Piece::Point{1, 1}, Piece::Point{1, -1}};

    CHECK(points == expected_points);
}

TEST_CASE("Square Rotation", "[square, rotation]") {
    Square s = Square(0);

    // 90 degrees clockwise
    s.rotate();
    std::vector<Piece::Point> points = s.get_points();
    std::vector<Piece::Point> expected_points = {Piece::Point{-1, -1}, Piece::Point{-1, 1},
        Piece::Point{1, 1}, Piece::Point{1, -1}};
    CHECK(points == expected_points);

    // 180 degrees clockwise
    s.rotate();
    points = s.get_points();
    CHECK(points == expected_points);

    // 270 degrees clockwise
    s.rotate();
    points = s.get_points();
    CHECK(points == expected_points);

    // 0 (360) degrees clocksise
    s.rotate();
    points = s.get_points();
    CHECK(points == expected_points);
}


TEST_CASE("Square Cloning", "[square, clone]") {
    Square s = Square(0);
    std::shared_ptr<Piece> s_clone = s.clone();
    REQUIRE(*s_clone.get() == s);
}

TEST_CASE("Piece Comparison", "[triangle, rectangle, square, comparison]") {

    // Triangle Comparisons
    Triangle t1 = Triangle(0);
    Triangle t2 = Triangle(0);
    Triangle t3 = Triangle(1);
    CHECK(t1 == t2);
    CHECK_FALSE(t1 == t3);

    t1.rotate();
    t3.rotate();
    CHECK_FALSE(t1 == t2);
    CHECK_FALSE(t3 == t2);

    t1.rotate();
    t1.rotate();
    t1.rotate();
    CHECK(t1 == t2);

    // Rectangle comparisons
    Rectangle r1 = Rectangle(0);
    Rectangle r2 = Rectangle(0);
    Rectangle r3 = Rectangle(1);
    CHECK(r1 == r2);
    CHECK_FALSE(r1 == r3);

    r1.rotate();
    CHECK_FALSE(r1 == r2);

    r1.rotate();
    r1.rotate();
    r1.rotate();
    CHECK(r1 == r2);

    // Square comparisons
    Square s1 = Square(0);
    Square s2 = Square(0);
    Square s3 = Square(1);
    CHECK(r1 == r2);
    CHECK_FALSE(r1 == r3);

    s1.rotate();
    CHECK(s1 == s2);
    CHECK_FALSE(s1 == s3);

    // Across piece comparisons
    CHECK_FALSE(t1 == r1);
    CHECK_FALSE(t1 == s1);
    CHECK_FALSE(r1 == s1);
}
