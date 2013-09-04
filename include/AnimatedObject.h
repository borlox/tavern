#ifndef ANIMATED_OBJECT_H
#define ANIMATED_OBJECT_H

#include "GameObject.h"

class AnimatedObject : public GameObject
{
	sf::Vector2i frameSize;

	size_t numFrames;
	size_t thisFrame;

	float frameTime;
	float thisTime;

	bool animate;

public:
	AnimatedObject()
	: frameSize(64, 96), numFrames(4), thisFrame(0), frameTime(0.2f), thisTime(0.f), animate(false)
	{ }

	AnimatedObject(sf::Texture* texture)
	: GameObject(texture)
	, frameSize(64, 96), numFrames(4), thisFrame(0), frameTime(0.2f), thisTime(0.f), animate(false)
	{ 
		SetFrame(0);
	}

	virtual void Update(float elapsed);

	void ResetAnimation()
	{
		SetFrame(0);
		thisTime = 0;
	}

	void Animate(bool anim = true)
	{
		animate = anim;
	}

private:
	void SetFrame(size_t n);
};

#endif //ANIMATED_OBJECT_H
