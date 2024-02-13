#ifndef square_hpp
#define square_hpp

#include "piece.hpp"

class Square: public Piece {
    
    public:

        // Creates a square with a specific owner based on the 
        // given `owner_id`. The initial orientation of the square is 
        // with the sides parallel to the board.
        Square(int owner_id);

        ~Square();
    
        // Rotates the square 90 degrees clockwise without changing the
        // position of the points.
        void rotate();

        // Returns a shared pointer to a clone of the square.
        std::shared_ptr<Piece> clone() const;

};

#endif
