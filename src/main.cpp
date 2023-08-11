#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <iostream>

#include "components/piece.hpp"
#include "components/triangle.hpp"
#include "components/square.hpp"
#include "components/rectangle.hpp"
#include "components/board.hpp"
#include "game.hpp"

#include "input/input_handler.hpp"

// Global input handler so it can be accessed in callbacks
InputHandler inputHandler;

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
        inputHandler.set_key_state(key, action == GLFW_PRESS || action == GLFW_REPEAT);
    }
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
    // Setup key handler
    std::vector<int> key_list = { // Keys we need to care about for the game
        GLFW_KEY_A,
        GLFW_KEY_W,
        GLFW_KEY_S,
        GLFW_KEY_D,
        GLFW_KEY_R,
        GLFW_KEY_ENTER,
        GLFW_KEY_SPACE
    };

    inputHandler = InputHandler(key_list);

    // Game Settings
    int blocks = 8;
    int num_players = 2;

    // Graphic Settings
    float block_width = 0.2f;

    // Game Objects
    Game game(blocks, num_players, &inputHandler);
    
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
    glfwSetKeyCallback(window, key_callback);

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
        game.progress_turn();
       
        // Debug placing all pieces
        Board temp;
        game.simulate_filling_placements(temp, 0);

        if (true) {
            std::cout << "drawing\n";
            draw_board_pieces(temp, blocks, block_width);
        } else {

        // Check if game is done
        Board final_board;
        if (game.check_if_game_is_finished(final_board)) {
            std::cout << "Game is complete!\n";
        }

        // Board and Cursor drawing
        Board board = game.get_board();
        Game::Cursor cursor = game.get_cursor();

        // Draw pieces of board and cursor
        draw_board_pieces(board, blocks, block_width);
        draw_cursor_piece(cursor, blocks, block_width);
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
