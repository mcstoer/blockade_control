#ifndef piece_hpp
#define piece_hpp

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>


class Piece {

    public: 
        
        // Type to represent 2d Point
        struct Point {
            int x;
            int y;
            friend auto operator<=>(const Point&, const Point&) = default;

            Point& operator+=(const Point& rhs) {
                x += rhs.x;
                y += rhs.y;
                return *this; 
            }
        };

        enum class piece_type {
            triangle,
            square,
            rectangle
        };

        enum class rotation {
            clockwise,
            counterclockwise
        };

        Piece(int owner_id);

        virtual ~Piece() {};

        std::vector<Point> get_points() const;

        piece_type get_piece_type() const;

        int get_owner_id() const;

        // Rotates the piece either clockwise or counterclockwise, with 
        // clockwise being default.
        void rotate(rotation rot = Piece::rotation::clockwise);

        virtual std::shared_ptr<Piece> clone() const = 0;

    protected: 
        std::vector<Point> points_;
        int rotation_; // Stores rotation in degrees
        piece_type piece_type_;
        int owner_id_; 
};

inline bool operator==(const Piece& lhs, const Piece& rhs) {
  
    // Check for matching type
    if (lhs.get_piece_type() != rhs.get_piece_type()) {
        return false;
    }

    // Note: checking for the same ownership doesn't work since when we 
    // compare pieces to see if they fit in the same slot as each other
    // we need to ignore the ownership.

    // Check for matching points.
    std::vector<Piece::Point> lhs_points = lhs.get_points();
    std::vector<Piece::Point> rhs_points = rhs.get_points();

    // Pieces with points in different orders are still the same piece.
    std::sort(lhs_points.begin(), lhs_points.end());
    std::sort(rhs_points.begin(), rhs_points.end());

    if (lhs_points.size() != rhs_points.size()) {
        return false;
    }

    for (int i = 0; i < lhs_points.size(); ++i) {
        if (lhs_points[i] != rhs_points[i]) {
            return false;
        }
    }

    return true;
}

#endif
