#ifndef player_hpp
#define player_hpp

#include <memory>

#include "actors/actor.hpp"
#include "input/input_handler.hpp"
#include "input/actions.hpp"

class Player : Actor {

    public: 

        Player(int id);

        ~Player();

        Action do_action();

        void set_input_handler(InputHandler* input_handler);


    private: 
        InputHandler* input_handler_;
};

#endif 
