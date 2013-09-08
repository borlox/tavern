#ifndef UNIT_CAMERA_H
#define UNIT_CAMERA_H

#include "sf_camera.h"
#include <cassert>

class GameObject;

class UnitCamera: public sftile::SfCamera
{
public:
	UnitCamera(const GameObject* unit, int tileWidth, int tileHeight, float width, float height);

	virtual void SetPosition(const float x, const float y)
	{
		position.x = x;
		position.y = y;
	}

	virtual void SetCenterPosition(const float x, const float y)
	{
		position.x = x - size.x/2.f;
		position.y = y - size.y/2.f;
	}

	virtual const sf::Vector2f GetPosition()
	{
		return position;
	}

	virtual const sf::Vector2f GetCenterPosition()
	{
		return position + 0.5f * size;
	}

	virtual void MoveTo(const float, const float)
	{
		assert(false && "UnitCamera::MoveTo should not be used");
	}

	virtual void MoveCenterTo(const float, const float)
	{
		assert(false && "UnitCamera::MoveCenterTo should not be used");
	}

	virtual void Update();
	virtual void HandleEvents(sf::Event& event);

private:
	const GameObject* unit;
	const int tileWidth;
	const int tileHeight;
};

#endif //UNIT_CAMERA_H
