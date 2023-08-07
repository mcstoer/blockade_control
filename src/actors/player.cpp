#include <GLFW/glfw3.h>

#include "actors/player.hpp"
#include "input/input_handler.hpp"


Player::Player(int id) : Actor(id) {};

Player::~Player() = default;

Action Player::do_action() {
   
    Action action = Action::NONE;

    if (input_handler_->get_key_state(GLFW_KEY_W)) {
        action = Action::UP;
    } else if (input_handler_->get_key_state(GLFW_KEY_A)) {
        action = Action::LEFT;
    } else if (input_handler_->get_key_state(GLFW_KEY_S)) {
        action = Action::DOWN;
    } else if (input_handler_->get_key_state(GLFW_KEY_D)) {
        action = Action::RIGHT;
    } else if (input_handler_->get_key_state(GLFW_KEY_R)) {
        action = Action::ROTATE;
    } else if (input_handler_->get_key_state(GLFW_KEY_F)) {
        action = Action::TOGGLE;
    } else if (input_handler_->get_key_state(GLFW_KEY_ENTER)) {
        action = Action::PLACE;
    }

    return action;
}

void Player::set_input_handler(InputHandler* input_handler) {
    input_handler_ = input_handler;
}
