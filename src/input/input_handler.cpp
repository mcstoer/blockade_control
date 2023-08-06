#include <input/input_handler.hpp>

InputHandler::InputHandler(std::vector<int> keys) {

    // Initialize state for given keys
    for (auto keyIter = keys.begin(); keyIter != keys.end(); ++keyIter) {
        keys_[*keyIter] = false;
    }
}

bool InputHandler::get_key_state(int key) const {
    
    if (auto search = keys_.find(key); search != keys_.end()) {
        return search->second;
    }

    return false;
}

void InputHandler::set_key_state(int key, bool isPressed) {
    keys_[key] = isPressed;
}
