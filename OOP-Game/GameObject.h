#pragma once

#include <SFML\Graphics.hpp>

#include <vector>
#include <map>
#include <string>

class State;
class Collider;

class GameObject : public sf::Transformable, public sf::Drawable {
private:
	friend class State;
	const State* m_parent;

	std::vector<const sf::Drawable*> m_graphics;
	std::vector<Collider*> m_colliders;
public:
	GameObject();
	~GameObject();

	virtual void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	virtual void collide(GameObject* other) { }

	//Returned the first graphic.
	const sf::Drawable* getGraphic();
	//This will set the graphic of the object to _graphic.
	//Warning this will replace the first element in the graphics list.
	void setGraphic(const sf::Drawable* _graphic);
	std::vector<const sf::Drawable*>* getGraphics();
	void addGraphic(const sf::Drawable* _graphic);

	//Returns the first collider in the m_colliders list.
	//Useful if GameObject only uses 1 collider (which is often the case).
	Collider* getCollider();
	void setCollider(Collider* collider);
	std::vector<Collider*>* getColliders();
	void addCollider(Collider* collider);

	//Helpful builder functions.
	void setHitbox(float width, float height, float offsetX = 0, float offsetY = 0);

};
