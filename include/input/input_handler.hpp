#ifndef input_handler_hpp
#define input_handler_hpp

#include <map>
#include <vector>

// Handles mouse and keyboard inputs by storing mouse and keyboard.
// This class is meant to be used in the glfw callback functions.
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

        // Gets the current mouse x position in the world space in terms of block widths.
        double get_mouse_xpos() const;

        // Gets the current mouse y position in the world space in terms of block widths.
        double get_mouse_ypos() const;

        // Sets the mouse x position in the world space in terms of block widths.
        void set_mouse_xpos(double xpos);

        // Sets the mouse y position in the world space in terms of block widths.
        void set_mouse_ypos(double ypos);

        // Gets the current y offset of the mouse's scroll wheel.
        double get_mouse_scroll_yoffset() const;

        // Sets the mouse's scroll wheel yoffset.
        void set_mouse_scroll_yoffset(double yoffset);

    private:
        handler_type keys_;
        double mouse_xpos_;
        double mouse_ypos_;
        double mouse_yoffset_;
};

#endif 
