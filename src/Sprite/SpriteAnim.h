#pragma once

#include <cmath>
#include <glad/glad.h>
#include <iostream>

namespace Sprite
{
struct SpriteSize
{
    float x, y;
    float width, height;
    //float widthTex, heightTex;
};

class SpriteAnim
{
    float currFrame{0};
    float lastUpTime{0};
    float animSpeed{0};
    unsigned int countFrames{0};

  public:
    SpriteAnim() = default;
    ~SpriteAnim() = default;
    SpriteAnim(float speed, float frames);
    SpriteAnim(const SpriteAnim &) = delete;
    SpriteAnim &operator=(const SpriteAnim &) = delete;
    SpriteAnim(SpriteAnim &&right) noexcept;
    SpriteAnim &operator=(SpriteAnim &&right) noexcept;
    void update(float curr_time);
    float get_curr_frame() const;
    unsigned int get_count_fr() const
    {
        return countFrames;
    };
    void set_curr_frame(float frame);
    void set_speed(float speed);
    void reset();
    bool is_animation_finished() const
    {
        return currFrame >= countFrames - 1;
    }
};
} // namespace Sprite