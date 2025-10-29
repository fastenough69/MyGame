#pragma once

#include "../Camera/Camera2D.h"
#include "../Render/IndexBuff.h"
#include "../Render/Shaders.h"
#include "../Render/SpriteAnim.h"
#include "../Render/Texture2D.h"
#include "../Render/VertexArr.h"
#include "../Render/VertexBuffArr.h"
#include "Background.h"
#include "Persons.h"

#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

using chahe_map = std::map<std::string, std::pair<Objects::BackroundLayer, std::shared_ptr<Render::Texture2D>>>;

namespace Objects
{
class GameObj
{
  protected:
    std::vector<float> ver{};
    std::vector<unsigned int> indices{};
    Render::VertexBuffArr vbo{};
    Render::IndexBuff emo{};
    Render::VertexArr vao{};
    GameObj() = default;
    virtual ~GameObj() = default;
    GameObj(std::vector<float> &&arr, std::vector<unsigned int> &&ids, Render::VertexBuffArr &&vb,
            Render::IndexBuff &&em, Render::VertexArr &&va)
        : ver{arr}, indices{ids}, vbo{std::move(vb)}, emo{std::move(em)}, vao{std::move(va)}
    {
    }

  public:
    virtual void init() = 0;
    virtual void render() = 0;
};

class GameObjMainHero : public GameObj
{
    std::shared_ptr<Objects::MainHero> object = nullptr;

  public:
    GameObjMainHero() = default;
    ~GameObjMainHero() = default;
    GameObjMainHero(const GameObjMainHero &) = delete;
    GameObjMainHero &operator=(const GameObjMainHero &) = delete;
    GameObjMainHero(GameObjMainHero &&right) = delete;
    GameObjMainHero &operator=(GameObjMainHero &&right) = delete;
    GameObjMainHero(std::shared_ptr<MainHero> objPtr, std::vector<float> &&arr, std::vector<unsigned int> &&ids,
                    Render::VertexBuffArr &&vb, Render::IndexBuff &&em, Render::VertexArr &&va);
    virtual void init() override;
    virtual void render() override;
    void update(float deltaTime, float &worldWidth, float &worldHeight, glm::mat4 &proj, glm::mat4 &view);
    glm::vec2 get_pos_obj()
    {
        return object->get_position();
    }
};

class GameObjBackground : public GameObj
{
    std::shared_ptr<Render::ProgramShader> shProg = nullptr;
    std::shared_ptr<Camera::Camera2D> cam = nullptr;
    chahe_map bg_map{};
    float repeatBg = 2.0f;
    void swap(GameObjBackground &obj) noexcept;

  public:
    GameObjBackground() = default;
    ~GameObjBackground()
    {
        bg_map.clear();
    };
    GameObjBackground(std::shared_ptr<Camera::Camera2D> cm, std::shared_ptr<Render::ProgramShader> ptr,
                      std::vector<float> &&arr, std::vector<unsigned int> &&ids, Render::VertexBuffArr &&vb,
                      Render::IndexBuff &&em, Render::VertexArr &&va);
    GameObjBackground(const GameObjBackground &);
    GameObjBackground &operator=(const GameObjBackground &);
    GameObjBackground(GameObjBackground &&) noexcept;
    GameObjBackground &operator=(GameObjBackground &&) noexcept;
    void add_layer(const std::string &name, BackroundLayer &lay, std::shared_ptr<Render::Texture2D> tex);
    virtual void init() override;
    virtual void render() override;
    void update_bg(const std::string &name);
    void multi_coord(float& curWorldWidht);
};
} // namespace Objects