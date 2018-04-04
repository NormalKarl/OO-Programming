#pragma once
//test
#include <sfml/graphics.hpp>

class GameObject;

//Instance of Axis is used for 
struct Axis {
	sf::Vector2f v1;
	sf::Vector2f v2;
};

class Collider
{
private:
	friend class GameObject;
	const GameObject* m_parent;

	sf::Vector2f m_origin;
	sf::Vector2f m_position;
	float m_rotation;
	bool m_immovable;
	float m_mass;
	float m_bounce;
	float m_friction;

	std::vector<Axis> m_axisPool;
	sf::FloatRect m_aabb;

	virtual void update() = 0;
public:
	Collider();
	virtual ~Collider();

	bool intersect(const Collider& other);
	void collide(Collider& other);
};

class BoxCollider : public Collider
{
private:
	sf::Vector2f m_halfSize;
public:
	BoxCollider(sf::Vector2f _halfSize);
	BoxCollider(float _halfWidth, float _halfHeight);
	
	void update();
};

class CircleCollider : public Collider {
private:
	float m_radius;
public:
	CircleCollider(float _radius);
};
