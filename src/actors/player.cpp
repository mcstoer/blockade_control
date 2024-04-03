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

    // Prioritize everything else over placements.
    // The scroll offsets are integer values but given as doubles from glfw.
    if (int(input_handler_->get_mouse_scroll_yoffset()) == -1) {
        action = Action::ROTATE_CLOCKWISE;
        // Reset scroll offset since it has been processed.
        input_handler_->set_mouse_scroll_yoffset(0);
    } else if (int(input_handler_->get_mouse_scroll_yoffset()) == 1) {
        action = Action::ROTATE_COUNTERCLOCKWISE;
        // Reset scroll offset since it has been processed.
        input_handler_->set_mouse_scroll_yoffset(0);
    } else if (input_handler_->get_key_state(GLFW_MOUSE_BUTTON_RIGHT)) {
        action = Action::TOGGLE;
    } else if (input_handler_->get_key_state(GLFW_MOUSE_BUTTON_LEFT)) {
        action = Action::PLACE;
    }

    return action;
}

void Player::set_input_handler(InputHandler* input_handler) {
    input_handler_ = input_handler;
}
