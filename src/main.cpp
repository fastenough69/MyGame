#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "Background/Background.h"
#include "Camera/Camera2D.h"
#include "Decor/Decor.h"
#include "Render/Shaders.h"
#include "Render/Texture2D.h"
#include "Resources/Resources.h"

static float window_SizeX = 320;
static float window_SizeY = 240;

int rand_(int min, int max)
{
    std::random_device rd;  // non-deterministic generator
    std::mt19937 gen(rd()); // to seed mersenne twister.
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

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

    /*Camera::Camera2D *obj = static_cast<Camera::Camera2D *>(glfwGetWindowUserPointer(pt_w));
    if (obj)
    {
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

    glfwSwapInterval(0);

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
        /*auto shProgramHero = mn->loadShaderPr("DefaultShaders", "res/shaders/ObjectsShaders/vShader.txt",
                                              "res/shaders/ObjectsShaders/fShader.txt");*/
        auto shProgramBg =
            mn->loadShaderPr("ShaderBg", "res/shaders/BgShaders/vShader.txt", "res/shaders/BgShaders/fShader.txt");

        if (!shProgramBg)
        {
            std::cerr << "Cant create program shaders" << std::endl;
        }
        /*auto run_tex = mn->loadTexture("Run_tex", "res/textures/RUN.png");
        auto idle_tex = mn->loadTexture("Idle_tex", "res/textures/IDLE.png");
        auto attack_tex = mn->loadTexture("Attack_tex", "res/textures/ATTACK 1.png");*/

        auto bg_tex1 = mn->loadTexture("Bg_tex", "res/textures/background_layer_1.png");
        auto bg_tex2 = mn->loadTexture("Bg_tex1", "res/textures/background_layer_2.png");
        auto bg_tex3 = mn->loadTexture("Bg_tex2", "res/textures/background_layer_3.png");

        auto tailset = mn->loadTexture("TailSet1", "res/textures/oak_woods_tileset.png");

        float worldWidth = 2000.0f;
        float worldHeight = (float)window_SizeY;

        float bgRepeatCount = 8.0f;
        std::vector<float> vecbg{0.0f,
                                 0.0f,
                                 0.0f,
                                 0.0f,
                                 0.0f,
                                 window_SizeX * bgRepeatCount,
                                 0.0f,
                                 0.0f,
                                 bgRepeatCount,
                                 0.0f,
                                 window_SizeX * bgRepeatCount,
                                 window_SizeY,
                                 0.0f,
                                 bgRepeatCount,
                                 1.0f,
                                 0.0f,
                                 window_SizeY,
                                 0.0f,
                                 0.0f,
                                 1.0f};

        std::shared_ptr<Camera::Camera2D> camera = std::make_shared<Camera::Camera2D>(window_SizeX, window_SizeY);
        Objects::BackgroundParalax bg{camera, shProgramBg, std::move(vecbg),
                                      std::vector<unsigned int>{0, 1, 2, 2, 3, 0}};
        bg.init();
        bg.add_layer("first", 0.2f, bg_tex1);
        bg.add_layer("sec", 0.5f, bg_tex2);
        bg.add_layer("thrid", 1.0f, bg_tex3);

        Objects::DecorObj first_floor{camera,
                                      shProgramBg,
                                      tailset,
                                      Objects::SizeTexture{120.0f, 169.0f, 71.0f, 23.0f, 504.0f, 360.0f},
                                      glm::vec2(0.0f, 23.0f),
                                      std::vector<unsigned int>{0, 1, 2, 2, 3, 0}};

        Objects::DecorObj sec_floor{camera,
                                    shProgramBg,
                                    tailset,
                                    Objects::SizeTexture{120.0f, 121.0f, 71.0f, 23.0f, 504.0f, 360.0f},
                                    glm::vec2(0.0f, 23.0f),
                                    std::vector<unsigned int>{0, 1, 2, 2, 3, 0}};

        std::map<int, Objects::DecorObj> tailmap_floor = {{1, first_floor}, {2, sec_floor}};
        std::vector<Objects::DecorObj> floor;
        for (int i{}; i < static_cast<unsigned int>(worldWidth / 71) + 1; i++)
        {
            int num = rand_(1, 2);
            floor.push_back(tailmap_floor[num]);
        }
        for (int i = 1; i < static_cast<unsigned int>(worldWidth / 71) + 1; i++)
        {
            std::vector<float> prev = floor[i - 1].get_vertecies(), curr = floor[i].get_vertecies();
            Objects::set_new_coord(prev, curr, floor[i - 1].get_szTexture().widht);
            floor[i].set_vertecies(curr);
        }
        for (int i{}; i < static_cast<unsigned int>(worldWidth / 71) + 1; i++)
        {
            floor[i].init();
        }

        std::vector<Objects::DecorObj> dirt{static_cast<unsigned int>(worldWidth / 71) + 1,
                                            {camera, shProgramBg, tailset,
                                             Objects::SizeTexture{120.0f, 73.0f, 71.0f, 23.0f, 504.0f, 360.0f},
                                             glm::vec2(0.0f, 0.0f), std::vector<unsigned int>{0, 1, 2, 2, 3, 0}}};
        auto ver_dirts = Objects::new_coords(dirt[0].get_vertecies(), static_cast<unsigned int>(worldWidth / 71) + 1);
        for (int i{}; i < dirt.size(); i++)
        {
            dirt[i].set_vertecies(ver_dirts[i]);
            dirt[i].init();
        }

        Objects::DecorObj ramp{camera,
                               shProgramBg,
                               tailset,
                               Objects::SizeTexture{0.0f, 193.0f, 95.0f, 47.0f, 504.0f, 360.0f},
                               glm::vec2(0.0f, 47.0f),
                               std::vector<unsigned int>{0, 1, 2, 2, 3, 0}};
        ramp.init();

        Objects::DecorObj platform{camera,
                                   shProgramBg,
                                   tailset,
                                   Objects::SizeTexture{0.0f, 264.0f, 96.0f, 96.0f, 504.0f, 360.0f},
                                   glm::vec2(650.0f, 0.0f),
                                   std::vector<unsigned int>{0, 1, 2, 2, 3, 0}};
        platform.init();

        Objects::DecorObj bl_sq{camera,
                                shProgramBg,
                                tailset,
                                Objects::SizeTexture{120.0f, 336.0f, 96.0f, 24.0f, 504.0f, 360.0f},
                                glm::vec2(630.0f, 46.0f),
                                std::vector<unsigned int>{0, 1, 2, 2, 3, 0}};
        bl_sq.init();

        glfwSetKeyCallback(pt_window, RightKeyCallback);

        float lastTime = 0;
        int frame = 0;
        glm::vec2 pos{0.0f, 0.0f};
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pt_window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            float currTime = glfwGetTime();
            float deltaTime = currTime - lastTime;
            lastTime = currTime;

            double x, y;
            glfwGetCursorPos(pt_window, &x, &y);
            x = (x / window_SizeX) * worldWidth;

            if (glfwGetKey(pt_window, GLFW_KEY_D) == GLFW_PRESS)
            {
                pos.x += 300.0f * deltaTime;
                if (pos.x >= worldWidth)
                    pos.x = worldWidth;
            }

            if (glfwGetKey(pt_window, GLFW_KEY_A) == GLFW_PRESS)
            {
                pos.x -= 300.0f * deltaTime;
                if (pos.x <= 0.0f)
                    pos.x = 0.0f;
            }

            camera->folow_target(pos, worldWidth, worldHeight);

            bg.update("first");
            bg.render();

            bg.update("sec");
            bg.render();

            bg.update("thrid");
            bg.render();

            platform.update();
            platform.render();

            bl_sq.update();
            bl_sq.render();

            for (int i{}; i < floor.size(); i++)
            {
                floor[i].update();
                floor[i].render();
            }
            for (int i{}; i < dirt.size(); i++)
            {
                dirt[i].update();
                dirt[i].render();
            }
            /*ramp.update();
            ramp.render();*/

            if (frame++ % 360 == 0)
            {
                //std::cout << pos.x << ' ' << pos.y << std::endl;
                // std::cout << frame / glfwGetTime() << std::endl;
                std::cout << x << ' ' << worldHeight - y << std::endl;
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(pt_window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}