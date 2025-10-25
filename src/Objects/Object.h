#pragma once

#include "../Render/SpriteAnim.h"
#include "../Render/Texture2D.h"
#include "../Render/Shaders.h"

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

using state_map = std::map<std::string, std::pair<std::shared_ptr<Render::Texture2D>, std::shared_ptr<Sprite::SpriteAnim>>>;

namespace Objects {
	class Object {
		std::shared_ptr<Render::ProgramShader> shProgram = nullptr;
		Sprite::SpriteSize size{};
		state_map table_state{};
		glm::vec2 position{};
		glm::vec2 oldPos{};
		glm::vec2 velocity{};
		float speed{ 200.0f };
		bool direction = true;
		bool is_attack = false;
	public:
		Object() = default;
		~Object() = default;
		Object(std::shared_ptr<Render::ProgramShader> pr, Sprite::SpriteSize& sz, glm::vec2& pos, glm::vec2& vel, float speed=200.0f);
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
		Object(Object&& right) noexcept;
		Object& operator=(Object&& right) noexcept;
		void set_pos(glm::vec2 ps);
		void add_state(const std::string name, std::shared_ptr<Render::Texture2D>, std::shared_ptr<Sprite::SpriteAnim>);
		void bind_state(const std::string name, std::vector<float>& ver);
		void move_object(int key, int action);
		void update(float deltaTime, float widht, float hight);
		glm::vec2 get_position() const { return position; }
		bool get_attack_flag() const { return is_attack; }
		void set_attack_flag(bool flg) { is_attack = flg; }
	};
}