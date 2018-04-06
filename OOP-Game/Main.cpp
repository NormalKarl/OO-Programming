#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "TileMap.h"
#include "Input.h"
#include <math.h>

#include "Game.h"
#include "State.h"
#include "Player.h"
#include "Button.h"

class PlayState : public State {
private:
	TileMap* map;
	Player* player;
	sf::Texture set;
	BitmapFont* bitmapFont;
	Button* btn;
	Camera camera;
	Camera gui_camera;
public:
	PlayState(std::string name) : State(name) {
		set = sf::Texture();
		set.loadFromFile("assets/level-tiles.png");
		map = new TileMap(16, 16, &set, 16, 32);

		for (int i = 0; i < 16; i++) {
			map->setTile(0, 0, i, 2, true);
		}

		for (int i = 8; i < 16; i++) {
			map->setTile(0, 0, i, 1, true);
		}

		player = new Player(map);

		bitmapFont = new BitmapFont("assets/font.fnt", { "assets/font_0.png" });
		btn = new Button(50, 25, bitmapFont, "Test Button");

		addGameObject(map);
		addGameObject(player);
		addGameObject(btn);
		camera = (Camera&)sf::View(sf::FloatRect(0, 0, 960, 540));

		camera.setFocused(player);
		gui_camera = (Camera&)sf::View(sf::FloatRect(0, 0, 960, 540));
		setCamera(&camera);
	}

	~PlayState() {
		delete map;
		delete player;
		delete bitmapFont;
		delete btn;
	}

};

void entry() {
	Game game = Game("Game Title", 960, 540);
	PlayState* state = new PlayState("PlayState");

	game.addState(state);
	game.start();
	delete state;
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