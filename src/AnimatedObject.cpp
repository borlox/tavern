#include "pch.h"
#include "AnimatedObject.h"

void AnimatedObject::Update(float elapsed)
{
	if (animate) {
		thisTime += elapsed;

		if (thisTime > frameTime) {
			thisTime -= frameTime;
			thisFrame = (thisFrame + 1) % numFrames;
			SetFrame(thisFrame);
		}
	}

	GameObject::Update(elapsed);
}

void AnimatedObject::SetFrame(size_t n)
{
	thisFrame = n;

	size_t row = 0;
	switch (GetOrientation()) {
	case South: row = 0; break;
	case West:  row = 1; break;
	case East:  row = 2; break;
	case North: row = 3; break;
	}

	sf::IntRect rect(thisFrame * frameSize.x, row * frameSize.y, frameSize.x, frameSize.y);
	SetSubRect(rect);
}
