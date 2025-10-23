#pragma once

#include <glad/glad.h>

namespace Sprite {
	struct SpriteSize {
		float x, y;
		float width, height;
	};

	class SpriteAnim {
		float currFrame{ 0 };
		float lastUpTime{ 0 };
		float animSpeed{ 0 };
		unsigned int countFrames{ 0 };
	public:
		SpriteAnim() = default;
		~SpriteAnim() = default;
		SpriteAnim(float speed, float frames);


		void update(float curr_time);
		float get_curr_frame() const;
		unsigned int get_count_fr() const { return countFrames; };
		void set_curr_frame(float frame);
		void set_speed(float speed);
	};
}