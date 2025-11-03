#pragma once

#include "../Camera/Camera2D.h"
#include "../Render/IndexBuff.h"
#include "../Render/Shaders.h"
#include "../Render/Texture2D.h"
#include "../Render/VertexArr.h"
#include "../Render/VertexBuffArr.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <memory>
#include <vector>

namespace Objects
{
class BackroundLayer
{
    float offsetLayer{};
    glm::vec2 pos{};

  public:
    BackroundLayer() = default;
    ~BackroundLayer() = default;
    BackroundLayer(float offset);

    void update(glm::vec2 &cameraPos);
    glm::mat4 get_model_bg() const;
};

using chahe_map = std::map<std::string, std::pair<Objects::BackroundLayer, std::shared_ptr<Render::Texture2D>>>;

class BackgroundParalax
{
    std::shared_ptr<Camera::Camera2D> cam = nullptr;
    std::shared_ptr<Render::ProgramShader> shProg = nullptr;
    chahe_map bg_map{};
    std::vector<float> ver{};
    std::vector<unsigned int> ids{};
    Render::VertexBuffArr vbo{};
    Render::IndexBuff emo{};
    Render::VertexArr vao{};

  public:
    BackgroundParalax() = default;
    ~BackgroundParalax() = default;
    BackgroundParalax(std::shared_ptr<Camera::Camera2D> cm, std::shared_ptr<Render::ProgramShader> prog,
                      std::vector<float> &&vr, std::vector<unsigned int> &&indices);

    void add_layer(const std::string &name, float&& offsetParalax, std::shared_ptr<Render::Texture2D> tex);
    void init();
    void update(const std::string &name);
    void render();
};
} // namespace Objects