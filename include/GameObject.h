#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
public:
	typedef std::unique_ptr<GameObject> Ptr;

	enum Orientation {
		North, East, South, West,
	};

public:
	GameObject()
	: orientation(South)
	{ }

	GameObject(sf::Texture* texture)
	:  orientation(South)
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

	virtual void Update(float elapsed)
	{ }

	void Render(sf::RenderTarget& target, sf::Vector2f screenPos)
	{
		if (texture) {
			sf::Sprite sprite(*texture, subRect);
			sprite.setPosition(screenPos);
			sprite.setOrigin(subRect.width * 0.5f, subRect.height * 0.9f);

			target.draw(sprite);
		}
	}

private:
	Orientation orientation;
	sf::Vector2f position;
	sf::Texture* texture;
	sf::IntRect subRect;
};

#endif //GAME_OBJECT_H
