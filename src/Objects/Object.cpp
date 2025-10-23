#include "Object.h"
#include <GLFW/glfw3.h>

Objects::Object::Object(std::shared_ptr<Render::ProgramShader> pr, Sprite::SpriteSize& sz, glm::vec2& pos, glm::vec2& vel, float sp) : 
	shProgram{pr}, size{sz}, position{pos}, velocity{vel}, speed{sp}
{
}

Objects::Object::Object(Object&& right) noexcept
{
	if (this == &right) return;
	shProgram = right.shProgram;
	table_state = right.table_state;
	right.shProgram = nullptr;
}

Objects::Object& Objects::Object::operator=(Object&& right) noexcept
{
	if (this == &right) return *this;
	shProgram = right.shProgram;
	table_state = right.table_state;
	right.shProgram = nullptr;
	return *this;
}

void Objects::Object::set_pos(glm::vec2 ps)
{
	position = ps;
}

void Objects::Object::add_state(const std::string name, std::shared_ptr<Render::Texture2D> tex, std::shared_ptr<Sprite::SpriteAnim> spr)
{
	if(table_state.count(name) == 0)
		table_state.emplace(name, std::pair(tex, spr));
}

void Objects::Object::bind_state(const std::string name, std::vector<float>& ver)
{
	if (table_state.count(name) == 0) return;
	auto pair = table_state[name];
	shProgram->usage();
	std::shared_ptr<Render::Texture2D> tex = pair.first;
	std::shared_ptr<Sprite::SpriteAnim> spr = pair.second;
	tex->bind();

	spr->update(glfwGetTime());
	float frameWidthUV = 1.0f / spr->get_count_fr();
	float uOffset = spr->get_curr_frame() * frameWidthUV;

	if (ver.size() >= 20) {
		ver[3] = uOffset;                 
		ver[8] = uOffset + frameWidthUV;    
		ver[13] = uOffset + frameWidthUV;    
		ver[18] = uOffset;              
	}
}

void Objects::Object::move_object(int key, int action)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W: velocity.y = speed; break;
		case GLFW_KEY_S: velocity.y = -speed; break;
		case GLFW_KEY_A: velocity.x = -speed; break;
		case GLFW_KEY_D: velocity.x = speed; break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_W: if (velocity.y > 0) velocity.y = 0; break;
		case GLFW_KEY_S: if (velocity.y < 0) velocity.y = 0; break;
		case GLFW_KEY_A: if (velocity.x < 0) velocity.x = 0; break;
		case GLFW_KEY_D: if (velocity.x > 0) velocity.x = 0; break;
		}
	}
}

void Objects::Object::update(float deltaTime, float widht, float height)
{
	position += velocity * deltaTime;
	if (position.x >= widht - size.width + 200.0f) position.x = widht - size.width + 200.0f;
	if (position.x <= 0.0f) position.x = 0.0f;
	if (position.y >= height - size.height) position.y = height - size.height;
	if (position.y <= 0.0f) position.y = 0.0f;
}