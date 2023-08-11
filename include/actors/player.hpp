#ifndef player_hpp
#define player_hpp

#include <memory>

#include "input/input_handler.hpp"
#include "input/actions.hpp"

class Player {

    public: 

        Player(int id, InputHandler* input_handler);

        ~Player();

        Action do_action();

        void set_input_handler(InputHandler* input_handler);

    private: 
        int id_;
        InputHandler* input_handler_;
};

#endif 
