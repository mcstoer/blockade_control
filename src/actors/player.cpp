#include <GLFW/glfw3.h>

#include "actors/player.hpp"
#include "input/input_handler.hpp"

Player::Player(int id, InputHandler* input_handler) {
    input_handler_ = input_handler;
};

Player::~Player() = default;

// Chooses an action based on the current key states of the input handler.
Action Player::do_action() {
   
    Action action = Action::NONE;

    // Priortize movement over other actions to ensure that
    // rotations, toggles and placements only happen
    // when stationary.
    if (input_handler_->get_key_state(GLFW_KEY_W)) {
        action = Action::UP;
    } else if (input_handler_->get_key_state(GLFW_KEY_A)) {
        action = Action::LEFT;
    } else if (input_handler_->get_key_state(GLFW_KEY_S)) {
        action = Action::DOWN;
    } else if (input_handler_->get_key_state(GLFW_KEY_D)) {
        action = Action::RIGHT;
    } else if (input_handler_->get_key_state(GLFW_KEY_R)
        || int(input_handler_->get_mouse_scroll_yoffset()) == -1) {
        action = Action::ROTATE_CLOCKWISE;
        // Reset scroll offset since it has been processed.
        input_handler_->set_mouse_scroll_yoffset(0);
    } else if (int(input_handler_->get_mouse_scroll_yoffset()) == 1) {
        action = Action::ROTATE_COUNTERCLOCKWISE;
        // Reset scroll offset since it has been processed.
        input_handler_->set_mouse_scroll_yoffset(0);
    } else if (input_handler_->get_key_state(GLFW_KEY_F)
        || input_handler_->get_key_state(GLFW_MOUSE_BUTTON_RIGHT)) {
        action = Action::TOGGLE;
    } else if (input_handler_->get_key_state(GLFW_KEY_ENTER)
        || input_handler_->get_key_state(GLFW_KEY_SPACE)
        || input_handler_->get_key_state(GLFW_MOUSE_BUTTON_LEFT)) {
        action = Action::PLACE;
    }

    return action;
}

void Player::set_input_handler(InputHandler* input_handler) {
    input_handler_ = input_handler;
}
