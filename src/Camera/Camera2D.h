#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Camera
{
class Camera2D
{
    glm::vec2 size{};
    glm::vec2 position{};

  public:
    Camera2D(float width, float height);
    Camera2D() = default;
    ~Camera2D() = default;

    void folow_target(glm::vec2 &target, float widht, float height);
    glm::mat4 get_view_matrix() const;
    glm::mat4 get_proj_matrix() const;
};
} // namespace Camera