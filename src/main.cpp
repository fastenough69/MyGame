#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "Camera/Camera2D.h"
#include "Render/IndexBuff.h"
#include "Render/Shaders.h"
#include "Render/Texture2D.h"
#include "Render/VertexArr.h"
#include "Render/VertexBuffArr.h"
#include "Resources/Resources.h"

float window_SizeX = 1600;
float window_SizeY = 900;

static void WindowSizeCallback(GLFWwindow *pt_w, int widht, int heigth)
{
    window_SizeX = widht;
    window_SizeY = heigth;
    glViewport(0, 0, window_SizeX, window_SizeY);
}

static void RightKeyCallback(GLFWwindow *pt_w, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pt_w, GL_TRUE);
    }

    /*Objects::MainHero *obj = static_cast<Objects::MainHero *>(glfwGetWindowUserPointer(pt_w));
    if (obj)
    {
        obj->move_object(key, action);
    }*/
}

int main(int argc, char **argv)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */

    /*auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window_SizeX = mode->width;
    window_SizeY = mode->height;*/

    std::cout << "Width: " << window_SizeX << std::endl;
    std::cout << "Height: " << window_SizeY << std::endl;

    GLFWwindow *pt_window = glfwCreateWindow(window_SizeX, window_SizeY, "Hello World", nullptr, nullptr);
    if (!pt_window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pt_window, WindowSizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pt_window);

    if (!gladLoadGL())
    {
        std::cout << "glad not found\n";
        return -1;
    }

    std::cout << "OpenGL version: " << GLVersion.major << '.' << GLVersion.minor << std::endl;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    {
        ResourceManager *mn = ResourceManager::getInstance(argv[0]);
        auto shProgramHero = mn->loadShaderPr("DefaultShaders", "res/shaders/ObjectsShaders/vShader.txt",
                                              "res/shaders/ObjectsShaders/fShader.txt");
        auto shProgramBg =
            mn->loadShaderPr("ShaderBg", "res/shaders/BgShaders/vShader.txt", "res/shaders/BgShaders/fShader.txt");

        if (!shProgramHero)
        {
            std::cerr << "Cant create program shaders" << std::endl;
        }
        auto run_tex = mn->loadTexture("Run_tex", "res/textures/RUN.png");
        auto idle_tex = mn->loadTexture("Idle_tex", "res/textures/IDLE.png");
        auto attack_tex = mn->loadTexture("Attack_tex", "res/textures/ATTACK 1.png");

        auto bg_tex1 = mn->loadTexture("Bg_tex", "res/textures/background_layer_1.png");
        auto bg_tex2 = mn->loadTexture("Bg_tex1", "res/textures/background_layer_2.png");
        auto bg_tex3 = mn->loadTexture("Bg_tex2", "res/textures/background_layer_3.png");

        float worldWidth = 3000.0f;
        float worldHeight = (float)window_SizeY;


        //glfwSetWindowUserPointer(pt_window, main_hero.get());
        glfwSetKeyCallback(pt_window, RightKeyCallback);

        float bgRepeatCount = 4.0f;
        std::vector<float>vecbg{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                        window_SizeX * bgRepeatCount, 0.0f, 0.0f, bgRepeatCount, 0.0f,
                        window_SizeX * bgRepeatCount, window_SizeY, 0.0f, bgRepeatCount, 1.0f,
                        0.0f, window_SizeY, 0.0f, 0.0f, 1.0f },
                                      

        float lastTime = 0;
        int frame = 0;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pt_window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            float currTime = glfwGetTime();
            float deltaTime = currTime - lastTime;
            lastTime = currTime;

            /*cam->folow_target(hero.get_pos_obj(), worldWidth, worldHeight);
            glm::mat4 proj = cam->get_proj_matrix();
            glm::mat4 view = cam->get_view_matrix();

            bg.update_bg("field");
            bg.render();

            bg.update_bg("first");
            bg.render();

            bg.update_bg("sec");
            bg.render();

            bg.update_bg("thrid");
            bg.render();

            hero.update(deltaTime, worldWidth, worldHeight, proj, view);
            hero.render();*/

            /*if (frame++ % 360 == 0)
            {
                std::cout << cam->get_cam_pos().x << ' ' << cam->get_cam_pos().y << ' ' << "curWorldHeight: " << ' ' <<
            worldWidth << std::endl;
            }*/

            /* Swap front and back buffers */
            glfwSwapBuffers(pt_window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}