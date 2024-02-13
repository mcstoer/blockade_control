#ifndef triangle_hpp
#define triangle_hpp

#include "piece.hpp"

class Triangle: public Piece {
    
    public:
        // Creates a half-square triangle with a specific owner based on the
        // given `owner_id`. The initial orientation of the triangle is
        // with the 90 degree vertex in the upper left corner.
        Triangle(int owner_id);

        ~Triangle();
    
        // Rotates the triangle 90 degrees clockwise by changing the
        // positions of the points.
        void rotate();

        // Returns a shared pointer to a clone of the triangle.
        std::shared_ptr<Piece> clone() const;
};

#endif
