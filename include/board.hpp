#include <array>
#include <memory>
#include <utility>
#include <components/piece.hpp>

class Board {

    public: 

        // The size of the board
        static constexpr int board_size = 8;

        using board_slot = typename std::pair<std::shared_ptr<Piece>,
                                              std::shared_ptr<Piece>>;

        // Store the board as a 2d Array of Piece pointers 
        // Need pointers to take advantage of abstract class
        using board_type = std::array<std::array<board_slot, board_size>, board_size>;

        using board_pointer = const std::array<board_slot, board_size>*;

        Board();

        const board_pointer get_board() const;

        // Attempts to place a given piece at a given x and y on the board
        // Precondition: x and y are in range [0,7]
        // If successful returns true, otherwise false
        bool place_piece(std::shared_ptr<Piece> piece, int x, int y);

        // Clears the board of all pieces and reverts it to the initial state.
        void clear();
       
    private:
        board_type board_;

        void place_initial_squares();

};
