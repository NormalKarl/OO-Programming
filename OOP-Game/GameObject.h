#pragma once

#include <SFML\Graphics.hpp>

#include <vector>
#include <map>
#include <string>

class InputState;

struct Flags
{
	bool top;

	union {
		struct {
			bool right;
			bool left;
		}; 

		bool sides;
	};

	union {
		bool bottom;
		bool grounded;
	};
};

class TileMap;

class GameObject : public sf::Transformable, public sf::Drawable {
private:
	sf::Texture texture;
	sf::IntRect textureRect;
	std::map<std::string, std::vector<sf::IntRect>> animations;

	sf::Vector2f hitbox;
	sf::Vector2f velocity;
public:
	GameObject();
	~GameObject();

	void addAnimation(std::string name, std::vector<int> frames, int frameWidth, int frameHeight);
	void play(std::string animationName);

	virtual void update() { }
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void collide(GameObject* other) { }

};

//Player

class Player : public GameObject
{
public:
	const InputState* right;
	const InputState* left;
	const InputState* jump;

	sf::Vector2f size;
	sf::RectangleShape shape;
	sf::Vector2f vel;
	bool grounded;

	Player();

	float applyPhysics(float vel, float accel, float drag, float max);
	void update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void collide(GameObject* other);
};