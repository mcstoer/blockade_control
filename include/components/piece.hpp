#ifndef piece_hpp
#define piece_hpp

#include <vector>

class Piece {

    public: 
        
        // Type to represent 2d Point
        struct Point {
            int x;
            int y;
        };

        std::vector<Point> get_points();

        virtual void rotate() = 0;

    protected: 
        std::vector<Point> points;
        int rotation; // Stores rotation in degrees
};

#endif
