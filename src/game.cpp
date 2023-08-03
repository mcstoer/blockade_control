#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <iostream>

#include "components/piece.hpp"
#include "components/triangle.hpp"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Given a pointer to a triangle and the board width in blocks
// draws the triangle
void draw_triangle(const Triangle* triangle, const int blocks, 
    const float block_width, const int board_x, const int board_y) {
    const std::vector<Piece::Point> points = triangle->get_points();

    const int center_offset = blocks / 2;

    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 1.0f, 0.0f);
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

void draw_piece(const Piece* piece, const int blocks,
    const float block_width, const int board_x, const int board_y) {

    if (piece->get_piece_type() == Piece::piece_type::triangle) {
        draw_triangle(dynamic_cast<const Triangle *>(piece), blocks,
            block_width, board_x, board_y);
    } else {
        assert(false);
    }
}

// Draw lines for game board
void draw_board_lines() {
    
    int lines = 9;
    int blocks = 8;

    float block_width = 0.2f;

    float total_width = block_width * blocks;

    float top = total_width / 2;
    float bottom = -top;

    std::vector<Piece*> pieces = {new Triangle()};

    draw_piece(pieces[0], blocks, block_width, 0, 0);
    draw_piece(pieces[0], blocks, block_width, 0, 7);
    draw_piece(pieces[0], blocks, block_width, 7, 0);
    draw_piece(pieces[0], blocks, block_width, 7, 7);

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

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(960, 720, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

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
        draw_board_lines();
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
