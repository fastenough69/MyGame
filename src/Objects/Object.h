#pragma once

#include "../Camera/Camera2D.h"
#include "../Render/IndexBuff.h"
#include "../Render/Shaders.h"
#include "../Render/SpriteAnim.h"
#include "../Render/Texture2D.h"
#include "../Render/VertexArr.h"
#include "../Render/VertexBuffArr.h"
#include "Persons.h"
// #include "Resources/Resources.h"

#include <memory>
#include <vector>

namespace Objects
{
class GameObjMainHero
{
    std::shared_ptr<Objects::MainHero> object = nullptr;
    std::vector<float> ver{};
    std::vector<unsigned int> indices{};
    Render::VertexBuffArr vbo{};
    Render::IndexBuff emo{};
    Render::VertexArr vao{};

  public:
    GameObjMainHero() = default;
    ~GameObjMainHero() = default;
    GameObjMainHero(const GameObjMainHero &) = delete;
    GameObjMainHero &operator=(const GameObjMainHero &) = delete;
    GameObjMainHero(GameObjMainHero &&right) noexcept;
    GameObjMainHero &operator=(GameObjMainHero &&right) noexcept;
    GameObjMainHero(std::shared_ptr<MainHero> objPtr, std::vector<float> &&arr, std::vector<unsigned int> &&ids,
                    Render::VertexBuffArr &&vb, Render::IndexBuff &&em, Render::VertexArr &&va);
    void init();
    void update(float deltaTime, float worldWidth, float worldHeight, glm::vec2 &diff_coord, glm::mat4 &proj,
                glm::mat4 &view);
    void render();
    glm::vec2 &get_pos_obj()
    {
        return object->get_position();
    }
};
} // namespace Objects