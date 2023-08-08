#ifndef game_hpp
#define game_hpp

#include <vector>
#include <memory>

#include "components/piece.hpp"
#include "components/board.hpp"
#include "input/input_handler.hpp"
#include "input/actions.hpp"
#include "actors/player.hpp"


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

        void progress_turn();

        Board get_board() const;

        Cursor get_cursor() const;

    private: 
        int blocks_;
        int num_actors_;
        int current_actor_turn_;
        std::vector<Player> players_;
        Board board_;
        InputHandler* input_handler_;
        Cursor current_cursor_;

        void reset_cursor(int id);

        void toggle_cursor_piece();

        // Gets the id of the actor who's turn is next
        int get_next_actor();
};

#endif 
