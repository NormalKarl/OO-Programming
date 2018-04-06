#pragma once
#include "GameObject.h"

//Player

class InputState;
class TileMap;

struct CollisionFlags {
	union {
		struct {
			bool left;
			bool right;
		};

		bool horizontal;
	};
	union {
		struct {
			bool top;

			union {
				bool bottom;
				bool grounded;
			};
		};

		bool vertical;
	};
};

class Player : public GameObject
{
public:
	TileMap* map;
	const InputState* right;
	const InputState* left;
	const InputState* jump;

	sf::Vector2f size;
	sf::RectangleShape shape;
	sf::Vector2f hitbox;
	sf::Vector2f vel;
	bool grounded;

	Player() {}
	Player(TileMap* map);

	float applyPhysics(float vel, float accel, float drag, float max);
	void update();
	void collide(GameObject* other);
};