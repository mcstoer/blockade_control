#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <iostream>

#include "components/piece.hpp"
#include "components/triangle.hpp"
#include "components/square.hpp"

// Structure for storing a colour
struct Color {
    GLfloat r, g, b;
};

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// TODO: Clean up drawing code to be able to draw triangles in a generic way
void draw_triangle_from_points(const std::vector<Piece::Point> points, const int blocks,
    const float block_width, const int board_x, const int board_y,
    const Color color) {

    const int center_offset = blocks / 2;

    glBegin(GL_TRIANGLES);
    glColor3fv(&color.r);

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
void draw_triangle(const Triangle* triangle, const int blocks, 
    const float block_width, const int board_x, const int board_y,
    const Color color) {
    
    const std::vector<Piece::Point> points = triangle->get_points();

    draw_triangle_from_points(points, blocks,
        block_width, board_x, board_y, color);
}

void draw_quad(const Piece* quad, const int blocks, 
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

void draw_piece(const Piece* piece, const int blocks,
    const float block_width, const int board_x, const int board_y) {

    Color player_color;

    if (piece->get_owner_id() == 0) {
        player_color = Color{1.0f, 0.0f, 0.0f};
    } else {
        player_color = Color{0.0f, 0.0f, 1.0f};
    }

    if (piece->get_piece_type() == Piece::piece_type::triangle) {
        draw_triangle(dynamic_cast<const Triangle *>(piece), blocks,
            block_width, board_x, board_y, player_color);
    } else if (piece->get_piece_type() == Piece::piece_type::square) {
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
    glColor3f(1.0f, 1.0f, 1.0f);

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

void draw_pieces(int blocks, float block_width) {
    std::vector<Piece*> pieces = {new Triangle(0), new Triangle(1),
        new Square(0), new Square(1)};

    draw_piece(pieces[0], blocks, block_width, 0, 0);
    pieces[0]->rotate();
    pieces[1]->rotate();
    draw_piece(pieces[1], blocks, block_width, 0, 7);
    pieces[0]->rotate();
    draw_piece(pieces[0], blocks, block_width, 7, 0);
    pieces[0]->rotate();
    draw_piece(pieces[0], blocks, block_width, 7, 7);
    pieces[0]->rotate();
    draw_piece(pieces[0], blocks, block_width, 4, 3);
    draw_piece(pieces[2], blocks, block_width, 4, 4);
    pieces[2]->rotate();
    draw_piece(pieces[2], blocks, block_width, 4, 5);
    draw_piece(pieces[3], blocks, block_width, 4, 6);

}

int main(void)
{
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

    // Game Settings
    int blocks = 8;

    // Graphic Settings
    float block_width = 0.2f;

    // Main rendering loop
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        
        // Main drawing
        draw_pieces(blocks, block_width);
        draw_board_lines(blocks, block_width);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
