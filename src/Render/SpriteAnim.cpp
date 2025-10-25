#include "SpriteAnim.h"

Sprite::SpriteAnim::SpriteAnim(float speed, float frames): animSpeed{speed}
{
	if (frames < 1.0f)
	{
		std::cerr << "frames not can smalest unit" << std::endl;
	}
	countFrames = frames;
}

void Sprite::SpriteAnim::update(float curr_time)
{
	if (curr_time - lastUpTime >= 1.0f / animSpeed)
	{
		currFrame = fmod((currFrame + 1), countFrames);
		lastUpTime = curr_time;
	}
}

float Sprite::SpriteAnim::get_curr_frame() const
{
	return currFrame;
}

void Sprite::SpriteAnim::set_curr_frame(float frame)
{
	currFrame = frame;
}

void Sprite::SpriteAnim::set_speed(float speed)
{
	animSpeed = speed;
}