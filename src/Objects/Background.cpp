#include "Background.h"

Objects::BackroundLayer::BackroundLayer(float offset) : offsetLayer{offset}
{
}

void Objects::BackroundLayer::update(glm::vec2& cameraPos)
{
	pos = cameraPos * offsetLayer;
}

glm::mat4 Objects::BackroundLayer::get_model_bg() const
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(-pos, 0.0f));
}