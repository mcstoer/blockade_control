#ifndef square_hpp
#define square_hpp

#include "piece.hpp"

class Square: public Piece {
    
    public:
        Square(int owner_id);

        ~Square();
    
       void rotate();

       std::shared_ptr<Piece> clone() const;

};

#endif
