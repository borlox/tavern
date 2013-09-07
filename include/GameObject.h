#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "ScriptingHelper.h"
#include "Tilemap.h"

class GameObject
{
public:
	typedef std::unique_ptr<GameObject> Ptr;

	enum Orientation {
		North, East, South, West,
	};

public:
	GameObject()
	: orientation(South), moveSpeed(2.0)
	{ }

	GameObject(sf::Texture* texture)
	:  orientation(South), moveSpeed(2.0)
	{ 
		SetTexture(texture);
	}

	void SetPosition(float x, float y)
	{
		SetPosition(sf::Vector2f(x, y));
	}

	void SetPosition(sf::Vector2f pos)
	{
		position = pos;
	}

	sf::Vector2f GetPosition() const
	{
		return position;
	}

	void SetOrientation(Orientation or)
	{
		orientation = or;
	}

	Orientation GetOrientation() const
	{
		return orientation;
	}

	void SetTexture(sf::Texture* tex)
	{
		texture = tex;
		if (tex) {
			subRect.left = 0;
			subRect.top = 0;
			subRect.width = tex->getSize().x;
			subRect.height = tex->getSize().y;
		}
	}

	void SetSubRect(sf::IntRect rect)
	{
		subRect = rect;
	}

	void FollowPath(Tilemap::Path p)
	{
		path = p;
	}

	void ResetPath()
	{
		path = Tilemap::Path();
	}

	virtual void Update(float elapsed);

	void Render(sf::RenderTarget& target, sf::Vector2f screenPos)
	{
		if (texture) {
			sf::Sprite sprite(*texture, subRect);
			sprite.setPosition(screenPos);
			sprite.setOrigin(subRect.width * 0.5f, subRect.height * 0.9f);

			target.draw(sprite);
		}
	}

	static luabind::scope ExportClass()
	{
		return 
			luabind::class_<ExportEnumDummy<Orientation>>("Orientation")
			.enum_("orientation") [
				luabind::value("North", GameObject::North),
				luabind::value("East",  GameObject::East),
				luabind::value("South", GameObject::South),
				luabind::value("West", GameObject::West)
			],
			luabind::class_<GameObject>("GameObject")
			.def("SetPosition", pick_overload(GameObject, SetPosition, void, (float, float)))
			.def("GetPosition", ::GetPosition<GameObject>)
			.def("SetOrientation", &GameObject::SetOrientation)
			.def("GetOrientation", &GameObject::GetOrientation)
			.def("FollowPath", &GameObject::FollowPath)
			;
	}

private:
	Orientation orientation;
	sf::Vector2f position;
	sf::Texture* texture;
	sf::IntRect subRect;

	float moveSpeed;

protected:
	Tilemap::Path path;
};

static inline GameObject::Orientation DirectionToOrientation(sf::Vector2f dir)
{
	if (abs(dir.x) > abs(dir.y)) {
		return dir.x >= 0 ? GameObject::East : GameObject::West;
	}
	else {
		return dir.y >= 0 ? GameObject::South : GameObject::North;
	}
}

#endif //GAME_OBJECT_H
