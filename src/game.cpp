#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Draw lines for game board
void draw_board_lines() {
    
    int lines = 9;
    float side_length = 2.0f - 0.2f;

    float top = side_length / 2 - 0.1f;
    float bottom = -top;

    glBegin(GL_LINES);

    // Try Vertical Lines First
    for (int i = -4; i <= 4; i++) {
        float horizontal_pos = side_length / lines  * i;
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
        
        draw_board_lines();
        /*
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glBegin(GL_LINES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-1.0f, -0.4f, 0.f);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-1.1f, -0.4f, 0.f);
        glEnd();
        */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
