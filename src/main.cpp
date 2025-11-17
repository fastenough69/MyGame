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
#include "Render/Shaders.h"
#include "Render/Texture2D.h"
#include "Resources/Resources.h"

static float window_SizeX = 720;
static float window_SizeY = 480;

void init_tiles()
{
    tmx::Map map;
    try
    {
        map.load("C:/Users/lisen/OneDrive/Рабочий стол/MyGame/res/resource/levels/1.tmx");
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }

    const auto &layers = map.getLayers();
    for (int i{}; i < layers.size(); i++)
    {
        if (layers[i]->getType() == tmx::Layer::Type::Tile)
        {
            const auto &tiles = layers[i]->getLayerAs<tmx::TileLayer>().getTiles();
            const auto &tileset = map.getTilesets()[0];
            const auto &sizeTile = map.getTileSize().x;
            const auto &sizeTex = tileset.getImageSize();
            auto &mapSize = map.getTileCount();
            std::cout << tileset.getFirstGID() << ' ' << tileset.getLastGID() << std::endl;
            std::vector<Objects::TileSize> sizesTiles;
            for (int y{}; y < (sizeTex.y / sizeTile); y++)
            {
                for (int x{}; x < (sizeTex.x / sizeTile); x++)
                {
                    sizesTiles.push_back(
                        Objects::TileSize{(float)(x * sizeTile), (sizeTex.y - sizeTile) - (float)(y * sizeTile),
                                          (float)sizeTile, (float)sizeTile, (float)(sizeTex.x), (float)(sizeTex.y)});
                    std::cout << tileset.getTiles()[y * (sizeTex.x / sizeTile) + x].ID << ' ';
                }
            }
        }
    }
}

glm::vec3 getWorldPosCursor(glm::vec3 mousePos, std::shared_ptr<Camera::Camera2D> cam)
{
    glm::vec4 viewPort{0, 0, window_SizeX, window_SizeY};
    return glm::unProject(mousePos, cam->get_view_matrix(), cam->get_proj_matrix(), viewPort);
}

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

    // glfwSwapInterval(0);

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
        auto shProgramBg =
            mn->loadShaderPr("ShaderBg", "res/shaders/BgShaders/vShader.txt", "res/shaders/BgShaders/fShader.txt");
        // mn->loadTileset("res\\resource\\levels\\1.tmx", "ts");
        mn->tileMapProcessing("res\\resource\\levels\\1.tmx", "ts");
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
        float worldX = 60.0f, worldY = 20.0f;
        float cameraX = 30;
        float bgRepeatCount = 8.0f;
        std::vector<float> vecbg{0.0f,
                                 0.0f,
                                 0.0f,
                                 0.0f,
                                 0.0f,
                                 cameraX * bgRepeatCount,
                                 0.0f,
                                 0.0f,
                                 bgRepeatCount,
                                 0.0f,
                                 cameraX * bgRepeatCount,
                                 worldY,
                                 0.0f,
                                 bgRepeatCount,
                                 1.0f,
                                 0.0f,
                                 worldY,
                                 0.0f,
                                 0.0f,
                                 1.0f};

        std::shared_ptr<Camera::Camera2D> camera = std::make_shared<Camera::Camera2D>(cameraX, worldY);
        Objects::BackgroundParalax bg{camera, shProgramBg, std::move(vecbg),
                                      std::vector<unsigned int>{0, 1, 2, 2, 3, 0}};
        bg.init();
        bg.add_layer("first", 0.2f, bg_tex1);
        bg.add_layer("sec", 0.5f, bg_tex2);
        bg.add_layer("thrid", 1.0f, bg_tex3);

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
            glm::vec3 mousePos{static_cast<float>(x), static_cast<float>(worldHeight - y), 0.0f};
            mousePos = getWorldPosCursor(mousePos, camera);

            if (glfwGetKey(pt_window, GLFW_KEY_D) == GLFW_PRESS)
            {
                pos.x += 10.0f * deltaTime;
                if (pos.x >= worldX)
                    pos.x = worldX;
            }

            if (glfwGetKey(pt_window, GLFW_KEY_A) == GLFW_PRESS)
            {
                pos.x -= 10.0f * deltaTime;
                if (pos.x <= 0.0f)
                    pos.x = 0.0f;
            }

            if (glfwGetKey(pt_window, GLFW_KEY_W) == GLFW_PRESS)
            {
                pos.y += 550 * deltaTime;
            }

            camera->folow_target(pos, worldWidth, worldHeight);

            bg.update("first");
            bg.render();

            bg.update("sec");
            bg.render();

            bg.update("thrid");
            bg.render();

            if (frame++ % 360 == 0)
            {
                /* std::cout << "\033[2J\033[1;1H";
                 std::cout << "World Pos: " << mousePos.x << ' ' << mousePos.y << std::endl;
                 std::cout << "Target pos: " << pos.x << ' ' << pos.y << std::endl;
                 std::cout << "Camera pos: " << camera->get_cam_pos().x << ' ' << camera->get_cam_pos().y << std::endl;
                 std::cout << "FPS: " << frame / glfwGetTime();
                 frame++;*/
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