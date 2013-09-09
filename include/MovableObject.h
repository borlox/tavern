#ifndef MOVABLE_OBJECT_H
#define MOVABLE_OBJECT_H

#include "AnimatedObject.h"

class MovableObject: public AnimatedObject
{
public:
	MovableObject()
	{ }

	MovableObject(sf::Texture* texture)
	: AnimatedObject(texture)
	{ }

	virtual void Update(float elapsed)
	{
		Animate(IsMoving());
		AnimatedObject::Update(elapsed);
	}

	bool IsMoving() const
	{
		return !path.empty();
	}

	static luabind::scope ExportClass()
	{
		return
			luabind::class_<MovableObject, AnimatedObject>("MovableObject")
			.def(luabind::constructor<>())
			.def(luabind::constructor<sf::Texture*>())
			.def("IsMoving", &MovableObject::IsMoving)
		;
	}
};

#endif //MOVABLE_OBJECT_H
