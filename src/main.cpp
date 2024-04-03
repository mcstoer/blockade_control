#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

#include "components/piece.hpp"
#include "components/triangle.hpp"
#include "components/square.hpp"
#include "components/rectangle.hpp"
#include "components/board.hpp"
#include "game.hpp"

#include "input/input_handler.hpp"

// Stores the data that's needed for callback functions. 
struct WindowData {
    InputHandler input_handler;
    float block_width;
};

// Structure for storing a colour
struct Color {
    GLfloat r, g, b, a;
};

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else {
        // Get the input handler from the window data. 
        WindowData* window_data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        window_data->input_handler.set_key_state(key, action == GLFW_PRESS || action == GLFW_REPEAT);
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Get the input handler from the window data. 
    WindowData* window_data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    // Either GLFW_PRESS or GLFW_RELEASE.
    window_data->input_handler.set_key_state(button, action == GLFW_PRESS);
}

// `xoffset` and `yoffset` are integer values of either -1, 0 or 1.
// `yoffset` is 1 for scrolling up, -1 for down and 0 for neutral.
// `xoffset` is 1 for moving to the right, -1 for left and 0 for neutral.
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Get the input handler from the window data. 
    WindowData* window_data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    // Either GLFW_PRESS or GLFW_RELEASE.
    window_data->input_handler.set_mouse_scroll_yoffset(yoffset);
}

// Converts screen space mouse coordinates to world space coordinates.
void convert_mouse_screen_coordinates_to_world_coordinates(double mouse_x, double mouse_y, double& world_xpos,
    double& world_ypos) {

    GLdouble model_view[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    GLdouble zpos;
    gluUnProject(mouse_x, mouse_y, 0.0f, model_view, projection, viewport, &world_xpos, &world_ypos,
        &zpos);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

    double world_xpos;
    double world_ypos;
    convert_mouse_screen_coordinates_to_world_coordinates(xpos, ypos, world_xpos, world_ypos);
    
    // Get the input handler and block width via the window data pointer.
    WindowData* window_data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

    // Scale the world coordinates by block width.
    window_data->input_handler.set_mouse_xpos(world_xpos / window_data->block_width);
    window_data->input_handler.set_mouse_ypos(world_ypos / window_data->block_width);
}


// Draws a triangle on the interface given three points, number of blocks on the board,
// the width of each block, the location of the triangle on the board and the color.
void draw_triangle_from_points(const std::vector<Piece::Point> points, const int blocks,
    const float block_width, const int board_x, const int board_y,
    const Color color) {

    const int center_offset = blocks / 2;

    glBegin(GL_TRIANGLES);
    glColor4fv(&color.r);

    for (auto pointIter = points.begin(); pointIter != points.end();
        ++pointIter) {
        
        // Pieces are centered at 0,0 and have a default width of 2
        Piece::Point p = *pointIter;

        // Need to scale down by half
        float rescaled_x = (p.x + 1.0f) / 2.0f;
        float rescaled_y = (p.y + 1.0f) / 2.0f;

        // Shift into position
        float new_x = (rescaled_x - center_offset + board_x) * block_width;

        // Need to offset by -1 for y due to 0,0 being top left for board
        // where top left of interface is -4 * board_width, 4 * board_width
        float new_y = (rescaled_y + center_offset - board_y - 1) * block_width;
        
        glVertex2f(new_x, new_y);
    }
    
    glEnd();
}

// Given a pointer to a triangle and the board width in blocks
// draws the triangle
void draw_triangle(const std::shared_ptr<Triangle> triangle, const int blocks, 
    const float block_width, const int board_x, const int board_y,
    const Color color) {
    
    const std::vector<Piece::Point> points = triangle->get_points();

    draw_triangle_from_points(points, blocks,
        block_width, board_x, board_y, color);
}

void draw_quad(const std::shared_ptr<Piece> quad, const int blocks, 
    const float block_width, const int board_x, const int board_y,
    const Color color) {

    const std::vector<Piece::Point> points = quad->get_points();

    // Form first triangle from first three points
    draw_triangle_from_points({points[0], points[1], points[2]}, blocks,
        block_width, board_x, board_y, color);

    // Form first triangle from last two points and first point
    draw_triangle_from_points({points[2], points[3], points[0]}, blocks,
        block_width, board_x, board_y, color);

}

void draw_piece(const std::shared_ptr<Piece> piece, const int blocks,
    const float block_width, const int board_x, const int board_y,
    const Color player_color) {


    if (piece->get_piece_type() == Piece::piece_type::triangle) {
        draw_triangle(std::dynamic_pointer_cast<Triangle>(piece), blocks,
            block_width, board_x, board_y, player_color);
    } else if (piece->get_piece_type() == Piece::piece_type::square || 
               piece->get_piece_type() == Piece::piece_type::rectangle) {
        draw_quad(piece, blocks,
            block_width, board_x, board_y, player_color);
    } else {
        assert(false);
    }
}

// Draw lines for game board
void draw_board_lines(int blocks, float block_width) {
    
    int lines = blocks + 1;

    float total_width = block_width * blocks;

    float top = total_width / 2;
    float bottom = -top;

    glBegin(GL_LINES);
    // Line colour
    glColor3f(0.0f, 0.0f, 0.0f);

    // Try drawing lines
    for (int i = -4; i <= 4; i++) {
        float horizontal_pos = block_width * i;
        glVertex2f(horizontal_pos, top);
        glVertex2f(horizontal_pos, bottom);

        // Horz
        glVertex2f(top, horizontal_pos);
        glVertex2f(bottom, horizontal_pos);
    }

    glEnd();
}

// Given an id returns the associated colour for that player
Color get_player_color(int id) {
    Color player_color;

    if (id == 0) {
        player_color = Color{1.0f, 0.0f, 0.0f, 1.0f};
    } else {
        player_color = Color{0.0f, 0.0f, 1.0f, 1.0f};
    }

    return player_color;
}

void draw_board_pieces(Board &board, int blocks, float block_width) {

   // Go through each slot on the board
   const Board::board_pointer board_array = board.get_board();

   for (int i = 0; i < Board::board_size; ++i) {
        for (int j = 0; j < Board::board_size; ++j) {

            // Draw pieces in each slot
            if (board_array[i][j].first) {
                const std::shared_ptr<Piece> first_piece = board_array[i][j].first;
                Color color = get_player_color(first_piece->get_owner_id());
                draw_piece(first_piece, blocks, block_width, j, i, color);
            }
            
            if (board_array[i][j].second) {
                const std::shared_ptr<Piece> second_piece = board_array[i][j].second;
                Color color = get_player_color(second_piece->get_owner_id());
                draw_piece(second_piece, blocks, block_width, j, i, color);
            }

        }
   }
}

void draw_cursor_piece(Game::Cursor& cursor, int blocks, float block_width) {
    Color color = get_player_color(cursor.player_id);
    // Lighten colours and add transparency
    color.r += 0.2f;
    color.g += 0.2f;
    color.b += 0.2f;
    color.a = 0.8f;
    draw_piece(cursor.piece, blocks, block_width, cursor.x, cursor.y, color);
}

int main(void)
{

    // Setup window data for callbacks.
    WindowData window_data;

    // Setup key handler
    std::vector<int> key_list = { // Keys we need to care about for the game
        GLFW_KEY_A,
        GLFW_KEY_W,
        GLFW_KEY_S,
        GLFW_KEY_D,
        GLFW_KEY_R,
        GLFW_KEY_ENTER,
        GLFW_KEY_SPACE,
        
        GLFW_MOUSE_BUTTON_LEFT,
        GLFW_MOUSE_BUTTON_RIGHT,
    };

    window_data.input_handler = InputHandler(key_list);

    // Game Settings
    int blocks = 8;
    int num_players = 2;

    // Graphic Settings
    float block_width = 0.2f;
    window_data.block_width = block_width;

    // Game Objects
    Game game(blocks, num_players, &window_data.input_handler);
   
    bool game_finished = false;
    bool end_game_stats_printed = false;
    Board final_board; // Store game end board

    // Create Window
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(960, 720, "Blockade Control", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // Add window data to window.
    glfwSetWindowUserPointer(window, &window_data);

    // Setup callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Setup alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(0.8f, 0.8f, 0.8f, 0.8f);

    // Main rendering loop
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);

        // Clear colours
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
       
        // Run game turns
        if (!game_finished) {

            // Check if game is done
            final_board.clear();
            game_finished = game.check_if_game_is_finished(final_board);
            game.progress_turn();
        
            // Board and Cursor drawing
            Board board = game.get_board();
            Game::Cursor cursor = game.get_cursor();

            // Draw pieces of board and cursor
            draw_board_pieces(board, blocks, block_width);
            draw_cursor_piece(cursor, blocks, block_width);
        } else {
            // Add a delay before updating to game end.
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            if (!end_game_stats_printed) {
                // Get the scores
                const float red_score = final_board.get_score(0);
                const int max_squares = Board::board_size * Board::board_size;
                const float blue_score = max_squares - red_score;

                if (red_score > blue_score) {
                    std::cout << "Red Wins!\n";
                } else if (red_score < blue_score) {
                    std::cout << "Blue Wins!\n";
                } else {
                    std::cout << "Tie!\n";
                }

                std::cout.precision(1);
                std::cout << "Score\n";
                std::cout << "Red: " << std::fixed << red_score << " Blue: " << blue_score << "\n";

                end_game_stats_printed = true;
            }

            draw_board_pieces(final_board, blocks, block_width);
        }
        // Draw lines of board and cursor
        draw_board_lines(blocks, block_width);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
