#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "TileMap.h"
#include "Input.h"
#include <math.h>

#include "Game.h"

void startGame() {
	Game game = Game("Game Title", 1280, 720);

	game.start();
}

#ifdef _WIN32
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	startGame();
	return 0;
}
#endif

void main() {
	startGame();


	/*sf::RenderWindow window(sf::VideoMode(1280, 720), "Running");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	sf::Event event;

	sf::Texture tile_set = sf::Texture();
	tile_set.loadFromFile("assets/level-tiles.png");
	TileMap map = TileMap(16, 16, &tile_set, 16, 32);

	for(int i = 0; i < 16; i++) {
		map.setTile(0, 0, i, 2, true);
	}
	for (int i = 8; i < 16; i++) {
		map.setTile(0, 0, i, 1, true);
	}

	Player player = Player();

	window.setKeyRepeatEnabled(false);

	while(window.isOpen()) {
		UpdateInput(KeyRight);
		UpdateInput(KeyLeft);
		UpdateInput(KeyJump);
		UpdateInput(KeyUp);
		UpdateInput(KeyDown);

		while(window.pollEvent(event)) {
			if(event.type == sf::Event::EventType::Closed) {
				//return 0;
				window.close();
			} else if(event.type == sf::Event::EventType::KeyPressed) {
				switch(event.key.code) {
					case sf::Keyboard::Right: KeyRight = InputState::Pressed; break;
					case sf::Keyboard::Left: KeyLeft = InputState::Pressed; break;
					case sf::Keyboard::Space: KeyJump = InputState::Pressed; break;
					case sf::Keyboard::Up: KeyUp = InputState::Pressed; break;
					case sf::Keyboard::Down: KeyDown = InputState::Pressed; break;
					default: break;
				}
			} else if(event.type == sf::Event::EventType::KeyReleased) {
				switch(event.key.code) {
					case sf::Keyboard::Right: KeyRight = InputState::Released; break;
					case sf::Keyboard::Left: KeyLeft = InputState::Released; break;
					case sf::Keyboard::Space: KeyJump = InputState::Released; break;
					case sf::Keyboard::Up: KeyUp = InputState::Released; break;
					case sf::Keyboard::Down: KeyDown = InputState::Released; break;
					default: break;
				}
			}

		}

		window.clear();
		player.update(0.0f);
		player.collide(map);
		sf::View view = window.getView();
		view.setCenter(player.shape.getPosition());
		window.setView(view);
		window.draw(map);
		window.draw(player);
		window.display();
	}*/
}