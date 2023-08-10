#ifndef rectangle_hpp
#define rectangle_hpp

#include "piece.hpp"

class Rectangle: public Piece {
    
    public:
        Rectangle(int owner_id);

        ~Rectangle();
    
        void rotate();

        std::shared_ptr<Piece> clone() const;
};

#endif
