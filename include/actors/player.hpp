#ifndef player_hpp
#define player_hpp

#include <memory>

#include "input/input_handler.hpp"
#include "input/actions.hpp"

// A class for representing the players of the game.
class Player {

    public: 

        // Creates a player with a given id and input handler.
        Player(int id, InputHandler* input_handler);
 
        ~Player();

        // Returns an action based on the input from the input handler.
        Action do_action();

        // Sets the player's input handler
        void set_input_handler(InputHandler* input_handler);

    private: 
        int id_;
        InputHandler* input_handler_;
};

#endif 
