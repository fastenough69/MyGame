#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Objects/Object.h"
#include "Render/IndexBuff.h"
#include "Render/Shaders.h"
#include "Render/SpriteAnim.h"
#include "Render/Texture2D.h"
#include "Render/VertexArr.h"
#include "Render/VertexBuffArr.h"
#include "Resources/Resources.h"
#include "Camera/Camera2D.h"

int window_SizeX = 1600;
int window_SizeY = 900;

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

    Objects::Object *obj = static_cast<Objects::Object *>(glfwGetWindowUserPointer(pt_w));
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
    /*glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(window_SizeX), 0.0f, static_cast<float>(window_SizeY), -1.0f, 1.0f);*/

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Сине-зеленый фон

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

        float worldWidth = 5000.0f;
        float worldHeight = 5000.0f;

        Sprite::SpriteSize szBg{-1.0f, -1.0f, (float)window_SizeX, (float)window_SizeY};
        Objects::Object backGr{shProgramBg, szBg, glm::vec2(szBg.x, szBg.y), glm::vec2(0.0f, 0.0f)};

        auto bg_spr = std::make_shared<Sprite::SpriteAnim>(1.0f, 1.0f);
        backGr.add_state("default", bg_tex, bg_spr);

        Sprite::SpriteSize sp{0.0f, 0.0f, 500, 500};
        Objects::Object main_hero(shProgramHero, sp, glm::vec2(sp.x, sp.y), glm::vec2(0.0f, 0.0f), 600.0f);

        glfwSetWindowUserPointer(pt_window, &main_hero);
        glfwSetKeyCallback(pt_window, RightKeyCallback);
        auto spr_run = std::make_shared<Sprite::SpriteAnim>(14.0f, 16.0f);
        auto spr_idle = std::make_shared<Sprite::SpriteAnim>(10.0f, 10.0f);
        auto spr_attack = std::make_shared<Sprite::SpriteAnim>(20.0f, 7.0f);

        main_hero.add_state(std::string("run_hero"), run_tex, spr_run);
        main_hero.add_state(std::string("idle_hero"), idle_tex, spr_idle);
        main_hero.add_state(std::string("attack_hero"), attack_tex, spr_attack);

        std::vector<float> ver = {0.0f,     0.0f,      0.0f, 0.0f, 0.0f, sp.width, 0.0f,      0.0f, 1.0f, 0.0f,
                                  sp.width, sp.height, 0.0f, 1.0f, 1.0f, 0.0f,     sp.height, 0.0f, 0.0f, 1.0f};

        // Попробуйте такие координаты для фона:k
        std::vector<float> verBg = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
            1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
            -1.0f, 1.0f,  0.0f, 0.0f, 1.0f  // top-left
        };

        
        //std::vector<float> verBg = {
        //    0.0f,       0.0f,        0.0f, 0.0f, 0.0f, // левый-низ
        //    worldWidth, 0.0f,        0.0f, 1.0f, 0.0f, // правый-низ
        //    worldWidth, worldHeight, 0.0f, 1.0f, 1.0f, // правый-верх
        //    0.0f,       worldHeight, 0.0f, 0.0f, 1.0f  // левый-верх
        //};

        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        Render::VertexBuffArr points_vbo;
        Render::IndexBuff emo;
        Render::VertexArr vaoHero;
        points_vbo.init(ver.data(), ver.size() * sizeof(float));
        vaoHero.init();
        vaoHero.add_buffer(points_vbo, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        vaoHero.add_buffer(points_vbo, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        emo.init(indices, sizeof(indices));
        vaoHero.unbind();
        shProgramHero->usage();
        shProgramHero->setInt("tex", 0);

        Render::VertexBuffArr points_bg;
        Render::IndexBuff emoBg;
        Render::VertexArr vaoBg;
        points_bg.init(verBg.data(), verBg.size() * sizeof(float));
        vaoBg.init();
        vaoBg.add_buffer(points_bg, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        vaoBg.add_buffer(points_bg, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        emoBg.init(indices, sizeof(indices));
        vaoBg.unbind();
        shProgramBg->usage();
        shProgramBg->setInt("tex", 0);

        shProgramHero->usage();
        GLuint modelLoc = glGetUniformLocation(shProgramHero->get_id(), "model");
        GLuint projLoc = glGetUniformLocation(shProgramHero->get_id(), "projection");
        GLuint viewLoc = glGetUniformLocation(shProgramHero->get_id(), "view");

        /*shProgramBg->usage();
        GLuint projLocBg = glGetUniformLocation(shProgramBg->get_id(), "projection");
        GLuint viewLocBg = glGetUniformLocation(shProgramBg->get_id(), "view");*/
        //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        Camera::Camera2D cam((float)window_SizeX, (float)window_SizeY);

        

        float lastTime = 0;
        glm::vec2 diff_coord = main_hero.get_position();
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pt_window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            float currTime = glfwGetTime();
            float deltaTime = currTime - lastTime;
            lastTime = currTime;

            main_hero.update(deltaTime, worldWidth, worldHeight);
            glm::vec2 pos_hero = main_hero.get_position();
            cam.folow_target(pos_hero, worldWidth, worldHeight);

            glm::mat4 projMat = cam.get_proj_matrix();
            glm::mat4 view = cam.get_view_matrix();

            /*shProgramBg->usage();
            glUniformMatrix4fv(projLocBg, 1, GL_FALSE, glm::value_ptr(projMat));
            glUniformMatrix4fv(viewLocBg, 1, GL_FALSE, glm::value_ptr(view));*/

            backGr.bind_state("default", verBg);
            vaoBg.bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            shProgramHero->usage();
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            if (main_hero.get_attack_flag())
            {
                main_hero.bind_state(std::string("attack_hero"), ver);
            }
            else if (main_hero.get_hitWall() || pos_hero != diff_coord)
            {
                main_hero.bind_state(std::string("run_hero"), ver);
            }
            else if (pos_hero == diff_coord)
            {
                main_hero.bind_state(std::string("idle_hero"), ver);
            }
            diff_coord = pos_hero;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(pos_hero.x, pos_hero.y, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            points_vbo.update_data(ver.data(), ver.size() * sizeof(float));

            shProgramHero->usage();
            vaoHero.bind();
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