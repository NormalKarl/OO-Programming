#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "TileMap.h"
#include "Input.h"
#include <math.h>

#include "Game.h"
#include "State.h"
#include "Button.h"

class Ball : public GameObject {
	sf::Vector2f vel;
	TileMap* map;

	float maxTime;
	float elapsed;
public:
	Ball(sf::Vector2f vel, TileMap* map) : GameObject("ball") {
		this->vel = vel;
		this->map = map;

		maxTime = 5.0f;
		elapsed = 0.0f;
	}

	void update() {
		GameObject::update();
		setBoundingBox(6, 6);

		elapsed += Game::getInstance().getDelta();

		if (elapsed > maxTime)
			destroy();

		bool t = false, r = false, l = false, b = false;
		collide(map, t, r, l, b);

		((sf::Sprite*)getGraphic())->setColor(sf::Color(255, 0, 0, 255));

		if (t) {
			vel = reflect(vel, { 0.0f, 1.0f });
		}
		else if (r) {
			vel = reflect(vel, { -1.0f, 0.0f });
		}
		else if (l) {
			vel = reflect(vel, { 1.0f, 0.0f });
		}
		else if (b) {
			vel = reflect(vel, { 0.0f, -1.0f });
		}
	}

	sf::Vector2f reflect(sf::Vector2f a, sf::Vector2f normal) {
		return a - 2.0f * dot(a, normal) * normal;
	}

	float dot(sf::Vector2f a, sf::Vector2f b) {
		return a.x * b.x + a.y * b.y;
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

	void collide(GameObject* other, bool& top, bool& right, bool& left, bool& bottom) {
		sf::Vector2f newVel = { vel.x, vel.y };

		if (dynamic_cast<TileMap*>(other) != NULL) {
			TileMap& tilemap = (TileMap&)*other;

			sf::Vector2f pos = getPosition() - getOrigin();

			sf::FloatRect original = sf::FloatRect(pos.x, pos.y, getBoundingBox().width, getBoundingBox().height);
			sf::FloatRect broadRect = Grow(original, vel);

			int minX = (int)floor((broadRect.left - tilemap.getPosition().x) / tilemap.getTileSize());
			int minY = (int)floor((broadRect.top - tilemap.getPosition().y) / tilemap.getTileSize());
			int maxX = (int)floor(((broadRect.left + broadRect.width) - tilemap.getPosition().x) / tilemap.getTileSize());
			int maxY = (int)floor(((broadRect.top + broadRect.height) - tilemap.getPosition().y) / tilemap.getTileSize());

			bool vertical = false, horizontal = false;

			for (int tileX = minX; tileX <= maxX; tileX++) {
				for (int tileY = minY; tileY <= maxY; tileY++) {
					if (tilemap.inBounds(tileX, tileY) && tilemap.isSolid(tileX, tileY)) {
						sf::FloatRect tileBounds = tilemap.getTileBounds(tileX, tileY);

						if (broadRect.intersects(tileBounds)) {
							//Ceiling
							if (!tilemap.isSolid(tileX, tileY + 1) && original.top >= tileBounds.top + tileBounds.height) {
								newVel.y = (tileBounds.top + tileBounds.height) - original.top;
								top = true;
							}
							//Floor
							else if (!tilemap.isSolid(tileX, tileY - 1) && original.top + original.height <= tileBounds.top) {
								newVel.y = tileBounds.top - (original.top + original.height);
								bottom = true;
							}
							//Left
							else if (!tilemap.isSolid(tileX + 1, tileY) && original.left >= tileBounds.left + tileBounds.width) {
								newVel.x = (tileBounds.left + tileBounds.width) - original.left;
								left = true;
							}
							//Right
							else if (!tilemap.isSolid(tileX - 1, tileY) && original.left + original.width <= tileBounds.left) {
								newVel.x = tileBounds.left - (original.left + original.width);
								right = true;
							}
						}
					}
				}
			}
		}

		setPosition(getPosition() + newVel);
	}
};

class Gun : public GameObject {
public:
	Gun() : GameObject("gun1") {
		setOrigin(5, 5);
	}
};

class PlayState : public State {
private:
	TileMap* m_map;

	std::vector<Ball*> m_activeBalls;
public:
	PlayState() {
		setCamera(sf::View({ 100, 175 }, { 200, 350 }));
		add(new GameObject("background"));

		m_map = new TileMap(10, 15, ResourceFactory::getInstance().getTexture("tiles"), 20, 20.0f);
		m_map->setPosition(0, 20);

		for (int x = 0; x < 10; x++) {
			m_map->setTile(0, 0, x, 0, true);
			m_map->setTile(0, 0, x, 14, true);
		}

		for (int y = 0; y < 15; y++) {
			m_map->setTile(0, 0, 0, y, true);
			m_map->setTile(0, 0, 9, y, true);
		}

		for (int i = 1; i < 5; i++)
			m_map->setTile(1, 0, i, i, true);

		add(m_map);
	}

	void update() {
		State::update();

		if (Input::getInstance().getState(sf::Mouse::Button::Left)->pressed()) {
			Gun* gun = new Gun();

			sf::Vector2f a = map(Input::getInstance().getMousePos()) - m_map->getPosition();

			a /= (float)m_map->getCellSize();

			a.x = floor(a.x);
			a.y = floor(a.y);
			a *= (float)m_map->getCellSize();

			gun->setPosition(a);
			add(gun);
			/*Ball* ball = new Ball({ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f }, m_map);

			ball->setPosition();
			

			m_activeBalls.push_back(ball);
			add(ball);*/
		}
	}



};

void entry() {
	Game::getInstance().setTitle("Round Shot");
	Game::getInstance().setSize(400, 700);
	ResourceFactory::getInstance().loadTexture("background", "assets/background.png");
	ResourceFactory::getInstance().loadTexture("tiles", "assets/tiles.png");
	ResourceFactory::getInstance().loadTexture("ball", "assets/ball.png");
	ResourceFactory::getInstance().loadTexture("gun1", "assets/Gun1.png");

	PlayState* playState = new PlayState();

	Game::getInstance().addState(playState);
	Game::getInstance().start();
}

#ifdef _WIN32
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	entry();
	return 0;
}

#endif

int main() {
	entry();
	return 0;
}
