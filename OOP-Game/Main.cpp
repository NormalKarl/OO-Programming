#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "TileMap.h"
#include "Input.h"
#include <math.h>

#include "Game.h"
#include "State.h"
#include "Player.h"
#include "Button.h"

class LevelData {

	TileMap* mainLevel;
	Player* player;
	std::vector<GameObject *> objects;

	void save(tinyxml2::XMLNode* node) {

	}

	void load(tinyxml2::XMLNode* node) {

	}

};

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

class TilePalleteObject : public GameObject {
private:
	const sf::Texture* m_texture;
	int m_cellSize;
	sf::Sprite** tiles;
	sf::RectangleShape m_border;
public:
	TilePalleteObject(const sf::Texture* _texture, int _cellSize) : m_texture(_texture), m_cellSize(_cellSize) {
		int noX = (int)m_texture->getSize().x / m_cellSize;
		int noY = (int)m_texture->getSize().y / m_cellSize;
		setBoundingBox(noX + (m_cellSize * noX) - 1, noY + (m_cellSize * noY) - 1);

		m_border.setPosition(-1, -1);
		m_border.setSize(sf::Vector2f(getBoundingBox().width + 2, getBoundingBox().height + 2));
		m_border.setFillColor(sf::Color(255, 255, 255, 50));
		m_border.setOutlineColor(sf::Color::Black);
		m_border.setOutlineThickness(1.0f);

		addGraphic(&m_border);

		tiles = new sf::Sprite*[noX * noY];

		for (int x = 0; x < noX; x++) {
			for (int y = 0; y < noX; y++) {
				sf::Sprite* sprite = new sf::Sprite();
				sprite->setTexture(*m_texture);
				sprite->setTextureRect({ x * m_cellSize, y * m_cellSize, m_cellSize, m_cellSize });
				sprite->setPosition((float)x + (x * m_cellSize), (float)y + (y * m_cellSize));
				tiles[(y * noX) + x] = sprite;
				addGraphic(sprite);
			}
		}

		setRelativeToView(false);
	}

};

class EditorState : public State {
private:

	enum class Tool {
		Pencil,
		Rubber,
		Select,
		Fill,
		Picker
	};

	enum class Panel {
		Pallete,
		Design
	};

	GameObject* toolbarContainer;
	ButtonGroup* toolGroup;
	Button* pencilButton;
	Button* rubberButton;
	Button* selectionButton;
	Button* fillButton;
	Button* eyeDropperButton;
	
	GameObject* panelContainer;
	ButtonGroup* panelGroup;
	Button* designButton;
	Button* palleteButton;

	TileMap* map;
	TilePalleteObject* tilePallete;

	const InputState* leftMouse;
	const InputState* middleMouse;
	const InputState* rightMouse;

	Tool tool;
	Panel panel;

	sf::Vector2i selectedTileSetTile;
public:
	EditorState(const AssetStore& store, std::string name) : State(name) {
		setClearColor(sf::Color(0, 88, 162));

		map = new TileMap(16, 16, store.tileSet, 16, 16);
		map->setGridVisible(true);
		addGameObject(map);

		toolbarContainer = new GameObject();

		for (int i = 0; i < 5; i++) {
			sf::Sprite* graphic = store.editorUI->makeSprite("ToolbarContainer");

			graphic->setPosition(0.0f, (float)0 + (i * 17));

			toolbarContainer->addGraphic(graphic);
		}

		toolbarContainer->setBoundingBox(18, 1 + (5 * 17));
		toolbarContainer->setPosition(3.0f, 3.0f);
		toolbarContainer->setRelativeToView(false);

		addGameObject(toolbarContainer);

		toolbarContainer->addChild(pencilButton = new Button(1, 1 + (0 * 17), store.editorUI, "Pencil", "PencilHover", "PencilDown", true));
		toolbarContainer->addChild(rubberButton = new Button(1, 1 + (1 * 17), store.editorUI, "Rubber", "RubberHover", "RubberDown", true));
		toolbarContainer->addChild(selectionButton = new Button(1, 1 + (2 * 17), store.editorUI, "Selection", "SelectionHover", "SelectionDown", true));
		toolbarContainer->addChild(fillButton = new Button(1, 1 + (3 * 17), store.editorUI, "PaintBucket", "PaintBucketHover", "PaintBucketDown", true));
		toolbarContainer->addChild(eyeDropperButton = new Button(1, 1 + (4 * 17), store.editorUI, "EyeDropper", "EyeDropperHover", "EyeDropperDown", true));
		toolGroup = new ButtonGroup({ pencilButton, rubberButton, selectionButton, fillButton, eyeDropperButton });
		toolGroup->select(pencilButton);

		panelContainer = new GameObject();
		const SpriteData* panelContainerData = store.editorUI->getSpriteData("PanelContainer");
		panelContainer->addGraphic(panelContainerData->makeSprite());
		panelContainer->setBoundingBox(panelContainerData->width, panelContainerData->height);
		panelContainer->setPosition((480 / 2) - (panelContainerData->width / 2), 3.0f);
		addGameObject(panelContainer);

		panelContainer->addChild(designButton = new Button(1, 1, store.editorUI, "DesignButton", "DesignHover", "DesignDown", true));
		panelContainer->addChild(palleteButton = new Button(32, 1, store.editorUI, "PalleteButton", "PalleteHover", "PalleteDown", true));
		panelGroup = new ButtonGroup({ designButton, palleteButton });
		panelGroup->select(designButton);

		setCamera(new Camera(480, 270)); 
		getCamera().setCenter(map->getPosition() + sf::Vector2f(map->getMapWidth() / 2 * map->getCellSize(), map->getMapHeight() / 2 * map->getCellSize()));
		leftMouse = Input::GetState(sf::Mouse::Left);
		middleMouse = Input::GetState(sf::Mouse::Middle);
		rightMouse = Input::GetState(sf::Mouse::Right);

		tilePallete = new TilePalleteObject(store.tileSet, 16);
		tilePallete->setVisible(false);

		float scale = 225.0f / tilePallete->getBoundingBox().height;
		tilePallete->setScale(scale, scale);

		tilePallete->setPosition((480 - (tilePallete->getBoundingBox().width * scale)) / 2, (270 - (tilePallete->getBoundingBox().height * scale)) / 2);

		addGameObject(tilePallete);


		selectedTileSetTile = { 0, 0 };
	}

	void update() {
		State::update();
		if (rightMouse->pressed()) {
			panelGroup->select(palleteButton);
			tilePallete->setVisible(true);
			map->setVisible(false);
			panel = Panel::Pallete;
		}
		
		if (rightMouse->released()) {
			panelGroup->select(designButton);
			tilePallete->setVisible(false);
			map->setVisible(true);
			panel = Panel::Design;
		}

		if (middleMouse->down()) {
			getCamera().setCenter(getCamera().getCenter() - getCamera().mapDistance(Input::GetMouseDelta()));
		};

		if (leftMouse->down()) {
			sf::Vector2f pos = getCamera().mapDistance(Input::GetMousePos());
			//printf("x: %f, y: %f\n", pos.x, pos.y);

			if (toolbarContainer->intersect(pos)) {
				if (pencilButton->isClicked()) {
					tool = Tool::Pencil;
				}
				else if (rubberButton->isClicked()) {
					tool = Tool::Rubber;
				}
				else if (selectionButton->isClicked()) {
					tool = Tool::Select;
				}
				else if (fillButton->isClicked()) {
					tool = Tool::Fill;
				}
				else if (eyeDropperButton->isClicked()) {
					tool = Tool::Picker;
				}
			}
			else if (panelContainer->intersect(pos)) {
				if (designButton->isClicked()) {
					panel = Panel::Design;
					tilePallete->setVisible(false);
					map->setVisible(true);
				}
				else if (palleteButton->isClicked()) {
					panel = Panel::Pallete;
					tilePallete->setVisible(true);
					map->setVisible(false);
				}
			}
			else {
				sf::Vector2f projectedPos = getCamera().mapPixelToCoords(Input::GetMousePos()) / (float)map->getCellSize();

				switch (tool) {
				case Tool::Pencil:
					map->setTile(0, 0, (int)projectedPos.x, (int)projectedPos.y, false);
					break;
				case Tool::Rubber:
					map->deleteTile((int)projectedPos.x, (int)projectedPos.y);
					break;
				case Tool::Picker:
					map->isTileSet((int)projectedPos.x, (int)projectedPos.y);
					break;
				}
			}
		}
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