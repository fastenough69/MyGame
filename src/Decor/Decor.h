#pragma once

#include "../Camera/Camera2D.h"
#include "../Render/IndexBuff.h"
#include "../Render/Shaders.h"
#include "../Render/Texture2D.h"
#include "../Render/VertexArr.h"
#include "../Render/VertexBuffArr.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace Objects
{
struct uvCoords
{
    float min_u, max_u;
    float min_v, max_v;
};

struct SizeTexture
{
    float x, y;
    float widht, height;
    float fullWidht, fullHeight;
};

static uvCoords get_uv_coords(const SizeTexture &size);
std::vector<std::vector<float>> new_coords(std::vector<float> first, unsigned int count);
void set_new_coord(const std::vector<float> &prev, std::vector<float> &curr, float widht);

class DecorObj
{
    std::shared_ptr<Camera::Camera2D> cam = nullptr;
    std::shared_ptr<Render::ProgramShader> shProg = nullptr;
    std::shared_ptr<Render::Texture2D> tex = nullptr;
    SizeTexture sizeTx{};
    uvCoords cord{};
    glm::vec2 point{};
    std::vector<float> ver{};
    std::vector<unsigned int> ids{};
    Render::VertexBuffArr vbo{};
    Render::IndexBuff emo{};
    Render::VertexArr vao{};
    void swap(DecorObj copy);
    void set_new_point(glm::vec2 mousePos, GLFWwindow *window);

  public:
    DecorObj() = default;
    ~DecorObj() = default;
    DecorObj(std::shared_ptr<Camera::Camera2D> cm, std::shared_ptr<Render::ProgramShader> prog,
             std::shared_ptr<Render::Texture2D> tx, SizeTexture &&size, glm::vec2 &&pt,
             std::vector<unsigned int> &&indices);
    DecorObj(const DecorObj &);
    DecorObj &operator=(const DecorObj &);
    DecorObj(DecorObj &&) noexcept;
    DecorObj &operator=(DecorObj &&) noexcept;
    void init();
    void update(glm::vec2 mousePos, GLFWwindow *window);
    void render();
    void set_vertecies(std::vector<float> &new_ver);
    std::vector<float> get_vertecies() const;
    void set_uvCord(SizeTexture &&size);
    SizeTexture get_szTexture() const;
};
} // namespace Objects