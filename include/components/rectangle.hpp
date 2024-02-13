#ifndef rectangle_hpp
#define rectangle_hpp

#include "piece.hpp"

class Rectangle: public Piece {
    
    public:
        // Creates a half-square rectangle with a specific owner based on the
        // given `owner_id`. The initial orientation of the triangle is
        // in the upper half of a square.
        Rectangle(int owner_id);

        ~Rectangle();
    
        // Rotates the rectangle 90 degrees clockwise by changing the
        // positions of the points.
        void rotate();

        // Returns a shared pointer to a clone of the triangle.
        std::shared_ptr<Piece> clone() const;
};

#endif
