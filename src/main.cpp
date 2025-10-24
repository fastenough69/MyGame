#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Render/Shaders.h"
#include "Resources/Resources.h"
#include "Render/Texture2D.h"
#include "Render/VertexBuffArr.h"
#include "Render/VertexArr.h"
#include "Render/IndexBuff.h"
#include "Render/SpriteAnim.h"
#include "Objects/Object.h"

int window_SizeX = 1920;
int window_SizeY = 1080;

unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

static void WindowSizeCallback(GLFWwindow* pt_w, int widht, int heigth)
{
    window_SizeX = widht;
    window_SizeY = heigth;
    glViewport(0, 0, window_SizeX, window_SizeY);
}

static void RightKeyCallback(GLFWwindow* pt_w, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pt_w, GL_TRUE);
    }

    Objects::Object* obj = static_cast<Objects::Object*>(glfwGetWindowUserPointer(pt_w));
    if (obj)
    {
        obj->move_object(key, action);
    }
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    
    auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window_SizeX = mode->width;
    window_SizeY = mode->height;

    std::cout << "Width: " << window_SizeX << std::endl;
    std::cout << "Height: " << window_SizeY << std::endl;

    GLFWwindow* pt_window = glfwCreateWindow(window_SizeX, window_SizeY, "Hello World", glfwGetPrimaryMonitor(), nullptr);
    if (!pt_window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pt_window, WindowSizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pt_window);

    if(!gladLoadGL())
    {
        std::cout << "glad not found\n";
        return -1;
    }

    std::cout << "OpenGL version: " << GLVersion.major << '.' << GLVersion.minor << std::endl;
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window_SizeX), 0.0f, static_cast<float>(window_SizeY), -1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Сине-зеленый фон

    {
        ResourceManager* mn = ResourceManager::getInstance(argv[0]);
        auto shProgram = mn->loadShaderPr("DefaultShaders", "res/shaders/vShader.txt", "res/shaders/fShader.txt");
        if (!shProgram)
        {
            std::cerr << "Cant create proram shaders" << std::endl;
        }
        auto run_tex = mn->loadTexture("Run_tex", "res/textures/RUN.png");
        auto idle_tex = mn->loadTexture("Idle_tex", "res/textures/IDLE.png");
            
        Render::VertexBuffArr points_vbo;
        Render::IndexBuff emo;
        Render::VertexArr vao;
        Sprite::SpriteSize sp{ 100, 100, 500, 500 };

        Objects::Object main_hero(shProgram, sp, glm::vec2(sp.x, sp.y), glm::vec2(0.0f, 0.0f), 700.0f);
        glfwSetWindowUserPointer(pt_window, &main_hero);
        glfwSetKeyCallback(pt_window, RightKeyCallback);
        auto spr = std::make_shared<Sprite::SpriteAnim>(14.0f, 16.0f);
        main_hero.add_state(std::string("run_hero"), run_tex, spr);

        std::vector<float> ver = { 0.0f, 0.0f, 0.0f,              0.0f, 0.0f,
                                    sp.width, 0.0f, 0.0f,         1.0f, 0.0f,
                                    sp.width, sp.height, 0.0f,    1.0f, 1.0f,
                                    0.0f, sp.height, 0.0f,          0.0f, 1.0f };
      
        points_vbo.init(ver.data(), ver.size() * sizeof(float));
        
        vao.init();
        vao.add_buffer(points_vbo, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        vao.add_buffer(points_vbo, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        emo.init(indices, sizeof(indices));
        vao.unbind();

        shProgram->usage();
        shProgram->setInt("tex", 0);

        GLuint modelLoc = glGetUniformLocation(shProgram->get_id(), "model");
        GLuint projLoc = glGetUniformLocation(shProgram->get_id(), "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        float lastTime = 0;
        float posX = 100;
        float posY = 100;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pt_window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            main_hero.bind_state(std::string("run_hero"), ver);

            glm::mat4 model = glm::mat4(1.0f);
            float currTime = glfwGetTime();
            float deltaTime = currTime - lastTime;
            lastTime = currTime;
            main_hero.update(deltaTime, window_SizeX, window_SizeY);

            glm::vec2 pos_hero = main_hero.get_position();
            model = glm::translate(model, glm::vec3(pos_hero.x, pos_hero.y, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            points_vbo.update_data(ver.data(), ver.size() * sizeof(float));

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