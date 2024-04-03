#include <input/input_handler.hpp>

InputHandler::InputHandler(std::vector<int> keys) {

    // Initialize state for given keys
    for (auto keyIter = keys.begin(); keyIter != keys.end(); ++keyIter) {
        keys_[*keyIter] = false;
    }
}

// Searches through the map of key presses to find the state of a key. 
bool InputHandler::get_key_state(int key) const {
    
    if (auto search = keys_.find(key); search != keys_.end()) {
        return search->second;
    }

    return false;
}

void InputHandler::set_key_state(int key, bool isPressed) {
    keys_[key] = isPressed;
}

double InputHandler::get_mouse_xpos() const {
    return mouse_xpos_;
}

void InputHandler::set_mouse_xpos(double xpos) {
    mouse_xpos_ = xpos;
}

double InputHandler::get_mouse_ypos() const {
    return mouse_ypos_;
}

void InputHandler::set_mouse_ypos(double ypos) {
    mouse_ypos_ = ypos;
}

double InputHandler::get_mouse_scroll_yoffset() const {
    return mouse_yoffset_;
}

void InputHandler::set_mouse_scroll_yoffset(double yoffset) {
    mouse_yoffset_ = yoffset;
}
