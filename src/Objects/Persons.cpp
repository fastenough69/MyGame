#include "Persons.h"

Objects::MainHero::MainHero(std::shared_ptr<Render::ProgramShader> pr, Sprite::SpriteSize &sz, glm::vec2 &pos,
                            glm::vec2 &vel, float sp)
    : Obj(pr, sz, pos)
{
    speed = sp;
    velocity = vel;
}

Objects::MainHero::MainHero(MainHero &&right) noexcept
{
    if (this == &right)
        return;
    shProgram = right.shProgram;
    table_state = right.table_state;
    size = right.size;
    position = right.position;
    oldPos = oldPos;
    velocity = right.velocity;
    speed = right.speed;
    direction = direction;
    right.shProgram = nullptr;
    right.table_state.clear();
}

Objects::MainHero &Objects::MainHero::operator=(MainHero &&right) noexcept
{
    if (this == &right)
        return *this;
    shProgram = right.shProgram;
    table_state = right.table_state;
    size = right.size;
    position = right.position;
    oldPos = oldPos;
    velocity = right.velocity;
    speed = right.speed;
    direction = direction;
    right.shProgram = nullptr;
    right.table_state.clear();
    return *this;
}

void Objects::MainHero::set_pos(glm::vec2 ps)
{
    position = ps;
}

void Objects::MainHero::add_state(const std::string name, std::shared_ptr<Render::Texture2D> tex,
                                  std::shared_ptr<Sprite::SpriteAnim> spr)
{
    if (table_state.count(name) == 0)
        table_state.emplace(name, std::pair(tex, spr));
}

void Objects::MainHero::bind_state(const std::string name, std::vector<float> &ver)
{
    if (table_state.count(name) == 0)
        return;
    auto pair = table_state[name];
    shProgram->usage();
    std::shared_ptr<Render::Texture2D> tex = pair.first;
    std::shared_ptr<Sprite::SpriteAnim> spr = pair.second;
    tex->bind();

    float currTimeAttack = glfwGetTime();
    if (name == "attack_hero")
    {
        if (!wasAttackign)
        {
            spr->reset();
            wasAttackign = true;
        }
        spr->update(currTimeAttack);
        if (spr->is_animation_finished())
        {
            is_attack = false;
            wasAttackign = false;
        }
        else
        {
            wasAttackign = false;
            spr->update(currTimeAttack);
        }
        wasAttackign = (name == "attack_hero");
    }

    spr->update(glfwGetTime());
    float frameWidthUV = 1.0f / spr->get_count_fr();
    float uOffset = spr->get_curr_frame() * frameWidthUV;

    if (ver.size() >= 20)
    {
        ver[3] = uOffset;
        ver[8] = uOffset + frameWidthUV;
        ver[13] = uOffset + frameWidthUV;
        ver[18] = uOffset;

        if (!direction)
        {
            ver[3] = 1.0f - ver[3];
            ver[8] = 1.0f - ver[8];
            ver[13] = 1.0f - ver[13];
            ver[18] = 1.0f - ver[18];
        }
    }
}

void Objects::MainHero::move_object(int key, int action)
{
    if (action == GLFW_PRESS)
    {

        switch (key)
        {
        case GLFW_KEY_A:
            velocity.x = -speed;
            direction = false;
            break;
        case GLFW_KEY_D:
            velocity.x = speed;
            direction = true;
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_A:
            if (velocity.x < 0)
                velocity.x = 0;

            break;
        case GLFW_KEY_D:
            if (velocity.x > 0)
                velocity.x = 0;
            break;
        }
    }

    if (key == GLFW_KEY_L)
    {
        if (action == GLFW_PRESS)
        {
            is_attack = true;
        }
        else
        {
            is_attack = false;
        }
    }
}

void Objects::MainHero::update(float deltaTime, float widht, float height)
{
    if (!is_attack)
    {
        position += velocity * deltaTime;
    }
    else
    {
        position += velocity * deltaTime * 0.3f;
    }

    bool hitwall = false;
    if (position.x >= widht - size.width)
    {
        position.x = widht - size.width;
        hitwall = true;
    }

    if (position.x <= 0.0f)
    {
        position.x = 0.0f;
        hitwall = true;
    }
    if (position.y >= height - size.height)
    {
        position.y = height - size.height;
        hitwall = true;
    }
    if (position.y <= 0.0f)
    {
        position.y = 0.0f;
        hitwall = true;
    }

    if (!hitwall)
    {
        if (oldPos.x > position.x)
        {
            direction = false;
        }
        else if (oldPos.x < position.x)
        {
            direction = true;
        }
    }
    oldPos = position;
    isRunningInPlace = hitwall && (velocity.x != 0.0f || velocity.y != 0.0f);
}