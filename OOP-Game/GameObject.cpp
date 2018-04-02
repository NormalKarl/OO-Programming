#include "GameObject.h"
#include "Input.h"
#include "TileMap.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::update(float delta) {}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	//target.draw();
}

//Player
Player::Player() {
	size = vel = {0, 0};
	shape = sf::RectangleShape({32, 64});
	shape.setPosition(0, 0);
	shape.setFillColor(sf::Color::White);
	grounded = false;
}

void Player::update(float delta) {
	float xAccel = 0;

	/*if(InputDown(KeyRight)) {
		xAccel = 1;
	} else if(InputDown(KeyLeft)) {
		xAccel = -1;
	}

	vel.x = applyPhysics(vel.x, xAccel, 0.3, 5);

	vel.y += 0.7f;

	if(InputPressed(KeyJump) && grounded) {
		vel.y = -10;
	}*/
}

sf::FloatRect Grow(sf::FloatRect rect, sf::Vector2f direction) {
	if(direction.x < 0) {
		float tempX = rect.left + rect.width;
		rect.left += direction.x;
		rect.width = tempX - rect.left;
	} else {
		rect.width += direction.x;
	}

	if(direction.y < 0) {
		float tempY = rect.top + rect.height;
		rect.top += direction.y;
		rect.height = tempY - rect.top;
	} else {
		rect.height += direction.y;
	}

	return rect;
}

float Player::applyPhysics(float vel, float accel, float drag, float max) {
	if (accel == 0) {
		if (abs(drag) > abs(vel)) {
			vel = 0;
		} else if (vel > 0) {
			vel -= drag;
		} else {
			vel += drag;
		}
	} else {
		vel += accel;
		
		if (vel > abs(max)) {
			vel = abs(max);
		} else if(vel < -abs(max)) {
			vel = -abs(max);
		}
	}

	return vel;
}

void Player::collide(TileMap& tilemap) {
	sf::FloatRect original = shape.getGlobalBounds();
	sf::FloatRect broadRect = Grow(shape.getGlobalBounds(), vel);
	sf::Vector2f newVel = {vel.x, vel.y};

	int minX = floor(broadRect.left / tilemap.getTileSize());
	int minY = floor(broadRect.top / tilemap.getTileSize());
	int maxX = floor((broadRect.left + broadRect.width) / tilemap.getTileSize());
	int maxY = floor((broadRect.top + broadRect.height) / tilemap.getTileSize());

	bool vertical = false, horizontal = false;

	grounded = false;

	for(int tileX = minX; tileX <= maxX; tileX++) {
		for(int tileY = minY; tileY <= maxY; tileY++) {
			if(tilemap.inBounds(tileX, tileY) && tilemap.isSolid(tileX, tileY)) {
				sf::FloatRect tileBounds = tilemap.getTileBounds(tileX, tileY);

				if(broadRect.intersects(tileBounds)) {
					if(!tilemap.isSolid(tileX, tileY + 1) && original.top >= tileBounds.top + tileBounds.height) {
						newVel.y = (tileBounds.top + tileBounds.height) - original.top;
						vertical = true;
					} else if(!tilemap.isSolid(tileX, tileY - 1) && original.top + original.height <= tileBounds.top) {
						newVel.y = tileBounds.top - (original.top + original.height);
						grounded = true;
						vertical = true;
					} else if(!tilemap.isSolid(tileX + 1, tileY) && original.left >= tileBounds.left + tileBounds.width) {
						newVel.x = (tileBounds.left + tileBounds.width) - original.left;
						horizontal = true;
					} else if(!tilemap.isSolid(tileX - 1, tileY) && original.left + original.width <= tileBounds.left) {
						newVel.x = tileBounds.left - (original.left + original.width);
						horizontal = true;
					}
				}
			}
		}
	}

	if(vertical)
		vel.y = 0;

	if(horizontal)
		vel.x = 0;

	setPosition(getPosition() + newVel);
	shape.setPosition(getPosition() - getOrigin());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(shape, states);
}