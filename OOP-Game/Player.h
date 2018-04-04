#pragma once
#include "GameObject.h"

//Player

class InputState;

class Player : public GameObject
{
public:
	const InputState* right;
	const InputState* left;
	const InputState* jump;

	sf::Vector2f size;
	sf::RectangleShape shape;
	sf::Vector2f hitbox;
	sf::Vector2f vel;
	bool grounded;

	Player();

	float applyPhysics(float vel, float accel, float drag, float max);
	void update();
	void collide(GameObject* other);
};