#ifndef game_hpp
#define game_hpp

#include <vector>
#include <memory>

#include "components/piece.hpp"
#include "components/board.hpp"
#include "input/input_handler.hpp"
#include "input/actions.hpp"
#include "actors/player.hpp"

// Runs the game by having players take turns and checks when the game is over.
class Game {

    public:
        // Stores the location that the player is interacting with
        struct Cursor {
            int x;
            int y;
            std::shared_ptr<Piece> piece;
            int player_id;
        };

        Game(int blocks, int num_players, InputHandler* input_handler);

        // Switches turns between the players.
        void progress_turn();

        Board get_board() const;

        Cursor get_cursor() const;
        
        // Checks if placing a given piece is valid for the current game state.
        bool check_if_valid_placement(std::shared_ptr<Piece> piece, int x, int y, 
            int half_squares_placed, const Board& target_board) const;

        // Check if the game is currently in a finished state.
        bool check_if_game_is_finished(Board& final_board);
        
        // Given a board, simulates placing as many pieces as possible for a given actor.
        void simulate_filling_placements(Board& board, int id);

    private:

        // Describes a position on the board and allows for comparison.
        struct board_pos {
            int i;
            int j;
            friend auto operator<=>(const board_pos&, const board_pos&) = default;
        };

        int blocks_;
        // Since only human players are currently implemented, actors and players 
        // mean the same thing.
        int num_actors_;
        int current_actor_turn_;
        int half_squares_placed_;
        std::vector<Player> players_;
        Board board_;
        InputHandler* input_handler_;
        Cursor current_cursor_;

        void reset_cursor(int id);

        // Switches the piece used for the cursor
        void toggle_cursor_piece();

        // Gets the id of the actor who's turn is next
        int get_next_actor();

        // Checks if a given piece can fit within a board slot.
        bool check_if_space_in_board_slot(std::shared_ptr<Piece> piece, int x, int y,
            const Board& target_board) const;

        // Checks if a player can place a piece based on pieces placed on
        // their turn.
        bool check_if_sufficient_half_squares_left(std::shared_ptr<Piece> piece,
            int half_squares_placed) const;

        // Checks if a piece placement would be connected to the player's other pieces.
        bool check_if_connected_to_existing_pieces(std::shared_ptr<Piece> piece, int x, int y,
            const Board& target_board) const;

        // Returns if any points in `points1` and `points2` are the same.
        bool compare_points(std::vector<Piece::Point> points1, std::vector<Piece::Point> points2) const;

        // Attempts to fill a slot by placing a new piece and returns true if 
        // successful.
        bool fill_slot(Board& board, const Board::board_slot& slot, int id, int i, int j);
};

#endif 
