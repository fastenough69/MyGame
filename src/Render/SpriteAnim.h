#pragma once

#include <glad/glad.h>

namespace Sprite {
	struct SpriteSize {
		float x, y;
		float width, height;
	};

	class SpriteAnim {
		unsigned int currFrame{ 0 };
		float lastUpTime{ 0 };
		float animSpeed{ 0 };
		unsigned int countFrames{ 0 };
	public:
		SpriteAnim() = default;
		~SpriteAnim() = default;
		SpriteAnim(float speed, unsigned int frames);


		void update(float curr_time);
		unsigned int get_curr_frame() const;
		void set_curr_frame(unsigned int frame);
		void set_speed(float speed);
	};
}