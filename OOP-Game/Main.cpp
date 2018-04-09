#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "TileMap.h"
#include "Input.h"
#include <math.h>

#include "Game.h"
#include "State.h"
#include "Player.h"
#include "Button.h"

struct AssetStore {
	sf::Texture* tileSet;
	SpriteSheet* editorUI;
	BitmapFont* squareFont;

	AssetStore() {
		tileSet = new sf::Texture();
		tileSet->loadFromFile("assets/level-tiles.png");
		editorUI = new SpriteSheet("assets/EditorUI.xml");
		squareFont = new BitmapFont("assets/font.fnt", { "assets/font_0.png" });

	};

	~AssetStore() {
		delete tileSet;
		delete editorUI;
		delete squareFont;
	}
};

class EditorState : public State {
private:
	Button* pencilButton;
	Button* rubberButton;
	Button* selectionButton;
	Button* fillButton;
	Button* eyeDropperButton;
	 
	GameObject* toolbarContainer;


	TileMap* map;

	const InputState* leftMouse;
public:
	EditorState(const AssetStore& store, std::string name) : State(name) {
		setClearColor(sf::Color(0, 88, 162));

		map = new TileMap(16, 16, store.tileSet, 16, 16);
		map->setGridVisible(true);
		addGameObject(map);


		toolbarContainer = new GameObject();

		for (int i = 0; i < 5; i++) {
			sf::Sprite* graphic = store.editorUI->makeSprite("ToolbarContainer");

			graphic->setPosition(3.0f, (float)3 + (i * 17));

			toolbarContainer->addGraphic(graphic);

		}
		toolbarContainer->setRelativeToView(false);

		addGameObject(toolbarContainer);

		pencilButton = new Button(store.editorUI->getSpriteData("Pencil"));
		rubberButton = new Button(store.editorUI->getSpriteData("Rubber"));
		selectionButton = new Button(store.editorUI->getSpriteData("Selection"));
		fillButton = new Button(store.editorUI->getSpriteData("PaintBucket"));
		eyeDropperButton = new Button(store.editorUI->getSpriteData("EyeDropper"));

		pencilButton->setPosition(1, 1 + (0 * 17));
		rubberButton->setPosition(1, 1 + (1 * 17));
		selectionButton->setPosition(1, 1 + (2 * 17));
		fillButton->setPosition(1, 1 + (3 * 17));
		eyeDropperButton->setPosition(1, 1 + (4 * 17));

		toolbarContainer->addChild(pencilButton);
		toolbarContainer->addChild(rubberButton);
		toolbarContainer->addChild(selectionButton);
		toolbarContainer->addChild(fillButton);
		toolbarContainer->addChild(eyeDropperButton);

		setCamera(new Camera(480, 270));

		leftMouse = Input::GetState(sf::Mouse::Left);
	}

	void update() {
		State::update();

		if (leftMouse->down()) {
			getCamera().setCenter(getCamera().getCenter() - getCamera().mapPixelToCoords((sf::Vector2i)Input::GetMouseDelta()));

			//printf("")
		};
	}

};

class PlayState : public State {
private:
	TileMap* map;
	Player* player;
	Button* btn;
	Camera camera;
	Camera gui_camera;
public:
	PlayState(const AssetStore& store, std::string name) : State(name) {
		map = new TileMap(16, 16, store.tileSet, 16, 32);

		for (int i = 0; i < 16; i++)
			map->setTile(0, 0, i, 2, true);

		for (int i = 8; i < 16; i++)
			map->setTile(0, 0, i, 1, true);

		player = new Player(map);

		btn = new Button(50, 25, store.squareFont, "Test Button");

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
		delete btn;
	}

};

void entry() {
	Game game = Game("Game Title", 960, 540);

	AssetStore store = AssetStore();

	PlayState* state = new PlayState(store, "Play");
	EditorState* editorState = new EditorState(store, "Editor");

	game.addState(editorState);
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