#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

#include "Render/Shaders.h"
#include "Resources/Resources.h"
#include "Render/Texture2D.h"
#include "Render/VertexBuffArr.h"
#include "Render/VertexArr.h"
#include "Render/IndexBuff.h"

int window_SizeX = 1280;
int window_SizeY = 960;

float vertices[] = {
    // positions          // texture coords
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,  // bottom left
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,  // bottom right
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,  // top right
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f   // top left
};

unsigned int indices[] = {
    0, 1, 2,  // first triangle
    2, 3, 0   // second triangle
};

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

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Сине-зеленый фон

    {
        ResourceManager* mn = ResourceManager::getInstance(argv[0]);
        auto shProgram = mn->loadShaderPr("DefaultShaders", "res/shaders/vShader.txt", "res/shaders/fShader.txt");
        if (!shProgram)
        {
            std::cerr << "Cant create proram shaders" << std::endl;
        }
        auto firstTex = mn->loadTexture("Default texture", "res/textures/IDLE.png");

        Render::VertexBuffArr points_vbo;
        Render::IndexBuff emo;
        Render::VertexArr vao;

        points_vbo.init(vertices, sizeof(vertices));
        emo.init(indices, sizeof(indices));
        vao.init();
        vao.add_buffer(points_vbo, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        vao.add_buffer(emo, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        vao.unbind();

        shProgram->usage();
        shProgram->setInt("tex", 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pt_window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shProgram->usage();
            firstTex->bind();

            vao.bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            /* Swap front and back buffers */
            glfwSwapBuffers(pt_window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}