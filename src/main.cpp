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

int window_SizeX = 1280;
int window_SizeY = 960;

float points[] = {
     -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f
};

float points1[] = {
     -0.5f, -0.5f, 0.0f,
     -0.5f, 0.5f, 0.0f,
     0.5f, 0.5f, 0.0f
};

float color[] = {
    1.0f, 0.5f, 0.0f,
    0.5f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

float color1[] = {
    1.0f, 0.5f, 0.0f,
    0.5f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

float tex_cord[] = {
    0.5f, 1.0f,     
    1.0f, 0.0f, 
    0.0f, 0.0f,
};

float tr1_x = 0.0f;
float tr1_y = 0.0f;
float tr2_x = 0.0f;
float tr2_y = 0.0f;
float speed = 0.5f;

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

    glClearColor(1, 0, 0, 1);

    {
        ResourceManager* mn = ResourceManager::getInstance(argv[0]);
        auto shProgram = mn->loadShaderPr("DefaultShaders", "res/shaders/vShader.txt", "res/shaders/fShader.txt");
        if (!shProgram)
        {
            std::cerr << "Cant create proram shaders" << std::endl;
        }
        auto firstTex = mn->loadTexture("Default texture", "res/textures/Brick_02-128x128.png");

        GLuint points_vbo1 = 0;
        glGenBuffers(1, &points_vbo1);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW);

        GLuint colors_vbo1 = 0;
        glGenBuffers(1, &colors_vbo1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color1), color1, GL_STATIC_DRAW);

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

        GLuint texture_vbo = 0;
        glGenBuffers(1, &texture_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_cord), tex_cord, GL_STATIC_DRAW);

        GLuint Vao = 0;
        glGenVertexArrays(1, &Vao);
        glBindVertexArray(Vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        GLuint Vao1 = 0;
        glGenVertexArrays(1, &Vao1);
        glBindVertexArray(Vao1);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(0);

        shProgram->usage();
        shProgram->setInt("tex", 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pt_window))
        {
            static float lastTime = glfwGetTime();
            float currTime = glfwGetTime();
            float deltaTime = currTime - lastTime;
            lastTime = currTime;

            tr1_x += speed * deltaTime;
            tr1_y = sin(currTime) * 0.3f;

            tr2_x += speed * deltaTime;
            tr2_y = cos(currTime) * 0.3f;

            if (tr1_x > 1.0f) tr1_x = -1.0f;
            if (tr2_x > 1.0f) tr2_x = -1.0f;

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shProgram->usage();
            firstTex->bind();

            GLuint modelLoc = glGetUniformLocation(shProgram->get_id(), "model");

            glm::mat4 model1 = glm::mat4(1.0f);
            //model1 = glm::rotate(model1, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model1 = glm::translate(model1, glm::vec3(tr1_x, tr1_y, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

            glBindVertexArray(Vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glm::mat4 model2 = glm::mat4(1.0f);
            model2 = glm::translate(model2, glm::vec3(tr1_x, tr1_y, 0.0f));
            model2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

            glBindVertexArray(Vao1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(pt_window);

            /* Poll for and process events */
            glfwPollEvents();

        }
    }

    glfwTerminate();
    return 0;
}