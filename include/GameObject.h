#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "ScriptingHelper.h"
#include "Path.h"

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

	void SetId(std::string str)
	{
		id = str;
	}

	std::string GetId() const
	{
		return id;
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

	void FollowPath(Path p, bool sendNotice)
	{
		path = p;
		noticeOnPathComplete = true;
	}

	void ResetPath()
	{
		path = Path();
		noticeOnPathComplete = false;
	}

	virtual void Update(float elapsed);

	void Render(sf::RenderTarget& target, sf::Vector2f screenPos) const
	{
		if (texture) {
			sf::Sprite sprite(*texture, subRect);
			sprite.setPosition(screenPos);
			sprite.setOrigin(subRect.width * 0.5f, subRect.height);

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
			.def(luabind::constructor<>())
			.def(luabind::constructor<sf::Texture*>())
			.def("SetPosition", pick_overload(GameObject, SetPosition, void, (sf::Vector2f)))
			.def("GetPosition", &GameObject::GetPosition)
			.def("SetOrientation", &GameObject::SetOrientation)
			.def("GetOrientation", &GameObject::GetOrientation)
			.def("SetId", &GameObject::SetId)
			.def("GetId", &GameObject::GetId)
			.def("FollowPath", &GameObject::FollowPath)
			;
	}

private:
	Orientation orientation;
	sf::Vector2f position;
	sf::Texture* texture;
	sf::IntRect subRect;

	std::string id;

	float moveSpeed;

protected:
	Path path;
	bool noticeOnPathComplete;
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
