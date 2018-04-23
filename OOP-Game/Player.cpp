#include "Player.h"
#include "Input.h"
#include "TileMap.h"
#include "Graphics.h"
#include "Resources.h"

struct Flags
{
	bool top;

	union {
		struct {
			bool right;
			bool left;
		};

		short sides;
	};

	union {
		bool bottom;
		bool grounded;
	};
};

//Player
Player::Player(TileMap* map) {
	this->map = map;
	jump = Input::GetState(sf::Keyboard::Key::Space);
	right = Input::GetState(sf::Keyboard::Key::Right);
	left = Input::GetState(sf::Keyboard::Key::Left);
	size = vel = { 0, 0 };
	shape = sf::RectangleShape({ 16, 32 });
	hitbox = { 16.0f, 32.0f };
	shape.setFillColor(sf::Color::White);
	grounded = false;

	setPosition(0, -100);

	//BitmapFont* font = new BitmapFont("assets/font.fnt", { "assets/font_0.png" });
	//addGraphic(new Label(font, "Hello, World"));

	addGraphic(&shape);
}

void Player::update() {
	GameObject::update();
	float xAccel = 0;

	if (right->down()) {
		xAccel = 1;
	}
	else if (left->down()) {
		xAccel = -1;
	}

	vel.x = applyPhysics(vel.x, xAccel, 0.3f, 5.0f);

	vel.y += 0.7f;

	if (jump->pressed() && grounded) {
		vel.y = -10;
	}

	collide(map);
}

sf::FloatRect Grow(sf::FloatRect rect, sf::Vector2f direction) {
	if (direction.x < 0) {
		float tempX = rect.left + rect.width;
		rect.left += direction.x;
		rect.width = tempX - rect.left;
	}
	else {
		rect.width += direction.x;
	}

	if (direction.y < 0) {
		float tempY = rect.top + rect.height;
		rect.top += direction.y;
		rect.height = tempY - rect.top;
	}
	else {
		rect.height += direction.y;
	}

	return rect;
}

float Player::applyPhysics(float vel, float accel, float drag, float max) {
	if (accel == 0) {
		if (abs(drag) > abs(vel)) {
			vel = 0;
		}
		else if (vel > 0) {
			vel -= drag;
		}
		else {
			vel += drag;
		}
	}
	else {
		vel += accel;

		if (vel > abs(max)) {
			vel = abs(max);
		}
		else if (vel < -abs(max)) {
			vel = -abs(max);
		}
	}

	return vel;
}

void Player::collide(GameObject* other) {
	sf::Vector2f newVel = { vel.x, vel.y };

	if (dynamic_cast<TileMap*>(other) != NULL) {
		TileMap& tilemap = (TileMap&)*other;

		sf::Vector2f pos = getPosition() - getOrigin();

		sf::FloatRect original = sf::FloatRect(pos.x, pos.y, hitbox.x, hitbox.y);
		sf::FloatRect broadRect = Grow(original, vel);

		int minX = (int)floor(broadRect.left / tilemap.getTileSize());
		int minY = (int)floor(broadRect.top / tilemap.getTileSize());
		int maxX = (int)floor((broadRect.left + broadRect.width) / tilemap.getTileSize());
		int maxY = (int)floor((broadRect.top + broadRect.height) / tilemap.getTileSize());

		bool vertical = false, horizontal = false;

		grounded = false;

		for (int tileX = minX; tileX <= maxX; tileX++) {
			for (int tileY = minY; tileY <= maxY; tileY++) {
				if (tilemap.inBounds(tileX, tileY) && tilemap.isSolid(tileX, tileY)) {
					sf::FloatRect tileBounds = tilemap.getTileBounds(tileX, tileY);

					if (broadRect.intersects(tileBounds)) {
						if (!tilemap.isSolid(tileX, tileY + 1) && original.top >= tileBounds.top + tileBounds.height) {
							newVel.y = (tileBounds.top + tileBounds.height) - original.top;
							vertical = true;
						}
						else if (!tilemap.isSolid(tileX, tileY - 1) && original.top + original.height <= tileBounds.top) {
							newVel.y = tileBounds.top - (original.top + original.height);
							grounded = true;
							vertical = true;
						}
						else if (!tilemap.isSolid(tileX + 1, tileY) && original.left >= tileBounds.left + tileBounds.width) {
							newVel.x = (tileBounds.left + tileBounds.width) - original.left;
							horizontal = true;
						}
						else if (!tilemap.isSolid(tileX - 1, tileY) && original.left + original.width <= tileBounds.left) {
							newVel.x = tileBounds.left - (original.left + original.width);
							horizontal = true;
						}
					}
				}
			}
		}

		if (vertical)
			vel.y = 0;

		if (horizontal)
			vel.x = 0;
	}

	setPosition(getPosition() + newVel);
}
