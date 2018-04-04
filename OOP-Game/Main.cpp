#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "TileMap.h"
#include "Input.h"
#include <math.h>

#include "Game.h"
#include "State.h"
#include "Player.h"

void entry() {
	Game game = Game("Game Title", 1280, 720);
	State* state = new State("Level");

	sf::Texture tile_set = sf::Texture();
	tile_set.loadFromFile("assets/level-tiles.png");
	TileMap* map = new TileMap(16, 16, &tile_set, 16, 32);

	for (int i = 0; i < 16; i++) {
		map->setTile(0, 0, i, 2, true);
	}
	for (int i = 8; i < 16; i++) {
		map->setTile(0, 0, i, 1, true);
	}

	Player* player = new Player();

	state->addGameObject(map);
	state->addGameObject(player);

	state->setCamera((Camera&)sf::View(sf::FloatRect(0, 0, 1280, 720)));
	state->getCamera().setFocused(player);

	game.addState(state);
	game.start();
	delete state;
	delete player;
	delete map;
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