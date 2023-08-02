#ifndef triangle_hpp
#define triangle_hpp

#include "piece.hpp"

class Triangle: public Piece {
    
    public:
        Triangle();

        ~Triangle();
    
        void rotate();

};

#endif
