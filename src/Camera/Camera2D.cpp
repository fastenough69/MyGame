#include "Camera2D.h"

Camera::Camera2D::Camera2D(float width, float height) : size{width, height}
{
}

void Camera::Camera2D::folow_target(glm::vec2 &target, float widht, float height)
{
    position.x = target.x - size.x / 2.0f;
    position.y = target.y - size.y / 2.0f;

    position.x = glm::clamp(position.x, 0.0f, widht - size.x);
    position.y = glm::clamp(position.y, 0.0f, height - size.y);
}

glm::mat4 Camera::Camera2D::get_view_matrix() const
{
    return glm::translate(glm::mat4(1.0f), glm::vec3(-position.x, -position.y, 0.0f));
}

glm::mat4 Camera::Camera2D::get_proj_matrix() const
{
    return glm::ortho(0.0f, size.x, 0.0f, size.y, -1.0f, 1.0f);
}
