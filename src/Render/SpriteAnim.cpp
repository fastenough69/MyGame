#include "SpriteAnim.h"

Sprite::SpriteAnim::SpriteAnim(float speed, unsigned int frames): animSpeed{speed}, countFrames(frames)
{

}

void Sprite::SpriteAnim::update(float curr_time)
{
	if (curr_time >= 1.0f / animSpeed)
	{
		currFrame = (currFrame + 1) % countFrames;
		lastUpTime = curr_time;
	}
}

unsigned int Sprite::SpriteAnim::get_curr_frame() const
{
	return currFrame;
}

void Sprite::SpriteAnim::set_curr_frame(unsigned int frame)
{
	currFrame = frame;
}

void Sprite::SpriteAnim::set_speed(float speed)
{
	animSpeed = speed;
}
