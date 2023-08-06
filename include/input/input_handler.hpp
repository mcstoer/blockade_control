#ifndef input_handler_hpp
#define input_handler_hpp

#include <map>
#include <vector>

class InputHandler {

    public:
        // Handler is a map of key presses
        using handler_type = std::map<int, bool>;

        InputHandler() = default;

        // Initializer handler with a given set of keys to consider
        InputHandler(std::vector<int> keys);

        // Get if a key is pressed 
        bool get_key_state(int key) const;

        // Set if a key is pressed
        void set_key_state(int key, bool isPressed);

    private:
        handler_type keys_;

};

#endif 
