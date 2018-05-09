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
	State* m_state;
	GameObject* m_parent;
	//const GameObject* m_parent;
	//std::vector<const Camera*> m_cameras; //These are the cameras in which this object will be drawn.
	std::vector<const sf::Drawable*> m_graphics;
	std::vector<GameObject*> m_children;
	sf::Vector2f aabb;
	sf::Vector2f aabbOrigin;

	bool m_usingCamera;

	int m_depth;
	bool m_persistent;
	bool m_visible;
public:
	GameObject(bool _persistent = false);
	virtual ~GameObject();

	//Returned the first graphic.
	const sf::Drawable* getGraphic();
	//This will set the graphic of the object to _graphic.
	//Warning this will replace the first element in the graphics list.
	void setGraphic(const sf::Drawable* _graphic);
	std::vector<const sf::Drawable*>* getGraphics();
	void addGraphic(const sf::Drawable* _graphic);

	void addChild(GameObject* _child);

	sf::FloatRect getBoundingBox();
	void setBoundingBox(float width, float height, float originX = 0, float originY = 0);
	virtual bool intersect(sf::Vector2f point);
	virtual bool intersect(GameObject* other);
	virtual bool intersect(sf::Vector2i point);
	virtual void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	inline bool isUsingCamera() { return m_usingCamera; }

	inline void setUsingCamera(bool _relativeToView) { this->m_usingCamera = _relativeToView; }

	inline bool isPersistent() const { return m_persistent; }

	inline void setPersistent(bool _persistent) { m_persistent = _persistent; }

	inline bool isVisible() const { return m_visible; }

	inline void setVisible(bool _visible) { m_visible = _visible; }

	inline int getDepth() const { return m_depth; }

	inline State* getState() { return m_state;  }

	void setDepth(int depth);

	sf::Vector2f getGlobalPosition();
};
