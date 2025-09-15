#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Render/Shaders.h"
#include "Resources/Resources.h"

int window_SizeX = 640;
int window_SizeY = 480;

float points[] = {
     0.0f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     -0.5f, -0.5f, 0.0f
};

float color[] = {
    1.0f, 0.5f, 0.0f,
    0.5f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

const char* vertext_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_pos;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"color = vertex_color;"
"gl_Position = vec4(vertex_pos, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"frag_color = vec4(color, 1.0);"
"}";

static void EscKeyCallback(GLFWwindow* pt_w, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pt_w, GL_TRUE);
    }
}

static void WindowSizeCallback(GLFWwindow* pt_w, int widht, int heigth)
{
    window_SizeX = widht;
    window_SizeY = heigth;
    glViewport(0, 0, window_SizeX, window_SizeY);
}

int main(int argc, char** argv)
{
    ResourceManager mn(argv[0]);
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    
    GLFWwindow* pt_window = glfwCreateWindow(window_SizeX, window_SizeY, "Hello World", nullptr, nullptr);
    if (!pt_window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pt_window, WindowSizeCallback);
    glfwSetKeyCallback(pt_window, EscKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pt_window);

    if(!gladLoadGL())
    {
        std::cout << "glad not found\n";
        return -1;
    }

    std::cout << "OpenGL version: " << GLVersion.major << '.' << GLVersion.minor << std::endl;

    glClearColor(1, 0, 0, 1);

    std::string verShader(vertext_shader), fragShader(fragment_shader);
    Render::ProgramShader shProgram(verShader, fragShader);
    if (!shProgram.get_compile())
    {
        std::cerr << "ERROR: CREATE SHADER PROGRAM\n";
        return -1;
    }

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    GLuint Vao = 0;
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pt_window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shProgram.usage();
        glBindVertexArray(Vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(pt_window);

        /* Poll for and process events */
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}