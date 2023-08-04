#include <array>
#include <memory>
#include <components/piece.hpp>

class Board {

    public: 

        // The size of the board
        static constexpr int board_size = 8;

        Board();

        // Attempts to place a given piece at a given x and y on the board
        // If successful returns true, otherwise false
        bool place_piece(std::unique_ptr<Piece> piece, int x, int y);

        // Clears the board of all pieces.
        void clear();
       
    private:
        // Store the board as a 2d Array of Piece pointers 
        // Need pointers to take advantage of abstract class
        std::array<std::array<std::unique_ptr<Piece>, board_size>, board_size> board_;


};
