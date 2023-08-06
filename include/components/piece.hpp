#ifndef piece_hpp
#define piece_hpp

#include <vector>

class Piece {

    public: 
        
        // Type to represent 2d Point
        struct Point {
            int x;
            int y;
            friend auto operator<=>(const Point&, const Point&) = default;
        };

        enum class piece_type {
            triangle,
            square,
            rectangle
        };

        Piece(int owner_id);

        virtual ~Piece() {};

        std::vector<Point> get_points() const;

        piece_type get_piece_type() const;

        int get_owner_id() const;

        virtual void rotate() = 0;

    protected: 
        std::vector<Point> points_;
        int rotation_; // Stores rotation in degrees
        piece_type piece_type_;
        int owner_id_; 
};

#endif
