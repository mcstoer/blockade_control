#ifndef actor_hpp
#define actor_hpp

#include "input/actions.hpp"

class Actor {

    public: 

        Actor(int id);

        virtual ~Actor() {};

        virtual Action do_action() = 0;

    protected: 
        int id_;

};

#endif 
