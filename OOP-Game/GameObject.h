#pragma once

#include <SFML\Graphics.hpp>

#include <vector>
#include <map>
#include <string>

class State;
class Collider;

/*
GameObject class holds only a very few general features.
It will be able to hold a list of sf::Drawable pointers which will
hold all the related graphics. It also has an Axis-Aligned Bounding Box (AABB)
which will be used to check for intersections against other GameObject's.
GameObject also inherits sf::Transformable (Abstract Class)
and sf::Drawable (Interface). sf::Transformable defines the position, origin,
scale and rotation of the GameObject, which produces a matrix for graphics manipulation.

Buttons are a good example of having an AABB and a graphic for the button.
*/
class GameObject : public sf::Transformable, public sf::Drawable {
private:
	friend class State;
	const State* m_parent;
	//std::vector<const Camera*> m_cameras; //These are the cameras in which this object will be drawn.
	std::vector<const sf::Drawable*> m_graphics;
	sf::Vector2f aabb;
	sf::Vector2f aabbOrigin;

	bool m_relativeToView;
public:
	GameObject();
	~GameObject();

	//Returned the first graphic.
	const sf::Drawable* getGraphic();
	//This will set the graphic of the object to _graphic.
	//Warning this will replace the first element in the graphics list.
	void setGraphic(const sf::Drawable* _graphic);
	std::vector<const sf::Drawable*>* getGraphics();
	void addGraphic(const sf::Drawable* _graphic);

	sf::FloatRect getBoundingBox();
	void setBoundingBox(float width, float height, float originX = 0, float originY = 0);
	virtual bool intersect(sf::Vector2f point);
	virtual bool intersect(GameObject* other);
	virtual bool intersect(sf::Vector2i point);
	virtual void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	inline bool isRelativeToView() {
		return m_relativeToView;
	}

	inline void setRelativeToView(bool _relativeToView) {
		this->m_relativeToView = _relativeToView;
	}
};
