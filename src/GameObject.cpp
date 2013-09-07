#include "pch.h"
#include "GameObject.h"

static float norm(const sf::Vector2f& v)
{
	return std::sqrt(v.x*v.x + v.y*v.y);
}

void GameObject::Update(float elapsed)
{
	if (!path.empty()) {
		sf::Vector2f tgt = path.top();
		sf::Vector2f dist = tgt - GetPosition();

		if (norm(dist) < 0.2)
			path.pop();
	}
	if (!path.empty()) {
		sf::Vector2f tgt = path.top();
		sf::Vector2f dir = tgt - GetPosition();

		float r = norm(dir);
		dir /= r;

		position += (dir * moveSpeed * elapsed);

		SetOrientation(DirectionToOrientation(dir));
	}
}
