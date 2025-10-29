#pragma once

#include "../Render/Shaders.h"
#include "../Render/Texture2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

        void update(glm::vec2& cameraPos);
        glm::mat4 get_model_bg() const;
    };
} // namespace Objects