#pragma once

#include "../Render/Shaders.h"
#include "../Render/SpriteAnim.h"
#include "../Render/Texture2D.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

using state_map =
    std::map<std::string, std::pair<std::shared_ptr<Render::Texture2D>, std::shared_ptr<Sprite::SpriteAnim>>>;

namespace Objects
{
class Obj
{
  protected:
    std::shared_ptr<Render::ProgramShader> shProgram = nullptr;
    Sprite::SpriteSize size{};
    state_map table_state{};
    glm::vec2 position{};
    
    Obj() = default;
    Obj(std::shared_ptr<Render::ProgramShader> pr, Sprite::SpriteSize &sz, glm::vec2 &pos)
        : shProgram{pr}, size{sz}, position{pos}
    {
    }

  public:
    virtual ~Obj()
    {
        table_state.clear();
    };
    virtual void set_pos(glm::vec2 ps) = 0;
    virtual void add_state(const std::string name, std::shared_ptr<Render::Texture2D>,
                           std::shared_ptr<Sprite::SpriteAnim>) = 0;
    virtual void bind_state(const std::string name, std::vector<float> &ver) = 0;
    virtual void update(float deltaTime, float widht, float hight) = 0;
    virtual glm::vec2 get_position() const = 0;
};

class Object : public Obj
{
    glm::vec2 oldPos{};
    glm::vec2 velocity{};
    float speed{200.0f};
    bool direction = true;
    bool is_attack = false;
    bool wasAttackign = false;
    bool isRunningInPlace = false;

  public:
    Object() = default;
    ~Object() = default;
    Object(std::shared_ptr<Render::ProgramShader> pr, Sprite::SpriteSize &sz, glm::vec2 &pos, glm::vec2 &vel,
           float speed = 200.0f);
    Object(const Object &) = delete;
    Object &operator=(const Object &) = delete;
    Object(Object &&right) noexcept;
    Object &operator=(Object &&right) noexcept;
    virtual void set_pos(glm::vec2 ps) override;
    virtual void add_state(const std::string name, std::shared_ptr<Render::Texture2D>,
                           std::shared_ptr<Sprite::SpriteAnim>) override;
    virtual void bind_state(const std::string name, std::vector<float> &ver) override;
    virtual void update(float deltaTime, float widht, float hight) override;
    virtual glm::vec2 get_position() const override
    {
        return position;
    };
    void move_object(int key, int action);
    bool get_attack_flag() const
    {
        return is_attack;
    }
    void set_attack_flag(bool flg)
    {
        is_attack = flg;
    }
    bool get_hitWall() const
    {
        return isRunningInPlace;
    }
};
} // namespace Objects