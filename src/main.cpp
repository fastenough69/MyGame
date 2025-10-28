#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Camera/Camera2D.h"
#include "Objects/Background.h"
#include "Objects/Object.h"
#include "Objects/Persons.h"
#include "Render/IndexBuff.h"
#include "Render/Shaders.h"
#include "Render/SpriteAnim.h"
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

    Objects::MainHero *obj = static_cast<Objects::MainHero *>(glfwGetWindowUserPointer(pt_w));
    if (obj)
    {
        obj->move_object(key, action);
    }
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // —ине-зеленый фон

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

        auto bg_tex = mn->loadTexture("Bg_tex", "res/textures/glacial_mountains_lightened.png");
        auto bg_tex1 = mn->loadTexture("Bg_tex1", "res/textures/clouds_bg.png");
        auto bg_tex2 = mn->loadTexture("Bg_tex2", "res/textures/sky_lightened.png");
        auto bg_tex3 = mn->loadTexture("Bg_tex3", "res/textures/clouds_mg_2.png");
        auto bg_tex4 = mn->loadTexture("Bg_tex4", "res/textures/clouds_mg_1_lightened.png");

        float worldWidth = 5000.0f;
        float worldHeight = (float)window_SizeY;

        Sprite::SpriteSize sp{0.0f, 0.0f, 200, 200};
        auto main_hero = std::make_shared<Objects::MainHero>(shProgramHero, sp, glm::vec2(sp.x, sp.y),
                                                             glm::vec2(0.0f, 0.0f), 600.0f);
        auto spr_run = std::make_shared<Sprite::SpriteAnim>(14.0f, 16.0f);
        auto spr_idle = std::make_shared<Sprite::SpriteAnim>(10.0f, 10.0f);
        auto spr_attack = std::make_shared<Sprite::SpriteAnim>(20.0f, 7.0f);

        main_hero->add_state(std::string("run_hero"), run_tex, spr_run);
        main_hero->add_state(std::string("idle_hero"), idle_tex, spr_idle);
        main_hero->add_state(std::string("attack_hero"), attack_tex, spr_attack);

        auto cam = std::make_shared<Camera::Camera2D>((float)window_SizeX, (float)window_SizeY);
        Objects::GameObjMainHero hero{main_hero,
                                      std::vector<float>{0.0f, 0.0f, 0.0f,      0.0f,     0.0f,      sp.width, 0.0f,
                                                         0.0f, 1.0f, 0.0f,      sp.width, sp.height, 0.0f,     1.0f,
                                                         1.0f, 0.0f, sp.height, 0.0f,     0.0f,      1.0f},
                                      std::vector<unsigned int>{0, 1, 2, 2, 3, 0},
                                      Render::VertexBuffArr{},
                                      Render::IndexBuff{},
                                      Render::VertexArr{}};
        hero.init();

        glfwSetWindowUserPointer(pt_window, main_hero.get());
        glfwSetKeyCallback(pt_window, RightKeyCallback);

        float bgRepeatCount = 4.0f; // сколько раз повтор€ть текстуру
        std::vector<float> verBg = {0.0f,
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

        Objects::GameObjBackground bg(cam, shProgramBg,
                                      std::vector<float>{0.0f,
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
                                                         1.0f},
                                      std::vector<unsigned int>{0, 1, 2, 2, 3, 0}, Render::VertexBuffArr{},
                                      Render::IndexBuff{}, Render::VertexArr{});
        bg.init();
        bg.add_layer("first", Objects::BackroundLayer{0.9f}, bg_tex);
        bg.add_layer("sec", Objects::BackroundLayer{0.8f}, bg_tex1);
        bg.add_layer("thrid", Objects::BackroundLayer{1.0f}, bg_tex2);
        bg.add_layer("four", Objects::BackroundLayer{0.6f}, bg_tex3);
        bg.add_layer("five", Objects::BackroundLayer{0.5f}, bg_tex4);
        float lastTime = 0;
        glm::vec2 diff_coord = main_hero->get_position();
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pt_window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            float currTime = glfwGetTime();
            float deltaTime = currTime - lastTime;
            lastTime = currTime;

            /*if (hero.get_pos_obj().x >= worldWidth - sp.width)
            {
                worldWidth += worldWidth;
            }*/

            cam->folow_target(hero.get_pos_obj(), worldWidth, worldHeight);
            glm::mat4 proj = cam->get_proj_matrix();
            glm::mat4 view = cam->get_view_matrix();

            bg.update_bg("thrid");
            bg.render();

            bg.update_bg("sec");
            bg.render();

            bg.update_bg("first");
            bg.render();

            bg.update_bg("four");
            bg.render();

            bg.update_bg("five");
            bg.render();

            hero.update(deltaTime, worldWidth, worldHeight, diff_coord, proj, view);
            hero.render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pt_window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}