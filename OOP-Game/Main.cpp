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
	sf::Texture* mainMenu;
	sf::Texture* infoBar;

	AssetStore() {
		tileSet = new sf::Texture();
		tileSet->loadFromFile("assets/level-tiles.png");
		editorUI = new SpriteSheet("assets/EditorUI.xml");
		squareFont = new BitmapFont("assets/font.fnt", { "assets/font_0.png" });
		mainMenu = new sf::Texture();
		mainMenu->loadFromFile("assets/mainmenu.png");
		infoBar = new sf::Texture();
		infoBar->loadFromFile("assets/bar.png");
	};

	~AssetStore() {
		delete tileSet;
		delete editorUI;
		delete squareFont;
		delete mainMenu;
	}
};

class TilePalleteObject : public GameObject {
private:
	const sf::Texture* m_texture;
	int m_cellSize;
	sf::Sprite** tiles;
	sf::RectangleShape m_border;
	int countX;
	int countY;
	sf::RectangleShape selectedShape;

	const InputState* leftButton;
public:
	TilePalleteObject(const sf::Texture* _texture, int _cellSize) : m_texture(_texture), m_cellSize(_cellSize) {
		int noX = (int)m_texture->getSize().x / m_cellSize;
		int noY = (int)m_texture->getSize().y / m_cellSize;
		setBoundingBox(noX + (m_cellSize * noX), noY + (m_cellSize * noY));

		countX = noX;
		countY = noY;

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
				sprite->setTextureRect({ (x * m_cellSize), (y * m_cellSize), m_cellSize, m_cellSize });
				sprite->setPosition((float)x + (x * m_cellSize), (float)y + (y * m_cellSize));
				tiles[(y * noX) + x] = sprite;
				addGraphic(sprite);
			}
		}


		selectedShape.setFillColor(sf::Color(255, 255, 255, 100));
		selectedShape.setOutlineColor(sf::Color(0, 0, 0, 150));
		selectedShape.setSize({ (float)m_cellSize, (float)m_cellSize });
		selectedShape.setOutlineThickness(1.0f);
		addGraphic(&selectedShape);

		leftButton = Input::GetState(sf::Mouse::Left);

		setRelativeToView(false);
	}

	bool selecting = false;
	sf::Vector2i startTile;
	sf::Vector2i endTile;

	void update() {
		sf::Vector2f pos = getState()->getCamera().mapDistance(Input::GetMousePos());

		if (isVisible() && intersect(pos)) {
			sf::Vector2i tileIndex = getTileIndex(pos);
			printf("%i, %i\n", tileIndex.x, tileIndex.y);

			if (tileIndex.x != -1 && tileIndex.y != -1) {
				if (leftButton->pressed()) {
					startTile = endTile = tileIndex;
				}
				else if (leftButton->down()) {
					if (selecting) {
						endTile = tileIndex;
					}
					else {
						startTile = tileIndex;
						selecting = true;
					}
				}
				else if (leftButton->released()) {
					selecting = false;
					sort(startTile, endTile);
				}

				updateSelectedRegion(startTile, endTile);
			}
		}
		else {
			selecting = false;
		}
	}

	void updateSelectedRegion(sf::Vector2i start, sf::Vector2i end) {
		sort(start, end);
		selectedShape.setPosition((sf::Vector2f)(start + (start * m_cellSize)));
		selectedShape.setSize((sf::Vector2f)((sf::Vector2i)(end - start) + (((end - start) + sf::Vector2i(1, 1)) * m_cellSize)));
	}

	void sort(sf::Vector2i& start, sf::Vector2i& end) {
		if (end.x < start.x) {
			int tempX = start.x;
			start.x = end.x;
			end.x = tempX;
		}

		if (end.y < start.y) {
			int tempY = start.y;
			start.y = end.y;
			end.y = tempY;
		}
	}

	void setSelectedPos(sf::Vector2i tileIndex) {
		selectedShape.setPosition(tileIndex.x + (tileIndex.x * m_cellSize), tileIndex.y + (tileIndex.y * m_cellSize));
	}

	int clamp(int value, int min, int max) {
		return value > max ? max : value < min ? min : value;
	}

	sf::Vector2i getTileIndex(sf::Vector2f pos) {
		pos -= getPosition();

		float sX = 1.0f / getScale().x;
		float sY = 1.0f / getScale().y;

		pos.x *= sX;
		pos.y *= sY;
		pos /= (float)m_cellSize + 1.0f;

		auto tileIndex = sf::Vector2i((int)floorf(pos.x), (int)floorf(pos.y));

		if (tileIndex.x < 0 || tileIndex.x >= countX || tileIndex.y < 0 || tileIndex.y >= countY) {
			return { clamp(tileIndex.x, 0, countX - 1), clamp(tileIndex.y, 0, countY - 1) };
		}

		return tileIndex;
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
	Panel m_panel;

	sf::Vector2i selectedTileSetTile;
public:
	EditorState(const AssetStore& store, std::string name) : State(name) {
		setClearColor(sf::Color(0, 88, 162));

		map = new TileMap(16, 16, store.tileSet, 16, 16);
		map->setGridVisible(true);
		addGameObject(map);

		PrimaryToolbarCreation: {
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

			//Create and add each button as a child to the toolbar.
			//Note: Children will always be sorted depth render ontop of the graphics of the parent.
			toolbarContainer->addChild(pencilButton = makeToggleButton(1, 1, store.editorUI, "Pencil"));
			toolbarContainer->addChild(rubberButton = makeToggleButton(1, 1 + (17 * 1), store.editorUI, "Rubber"));
			toolbarContainer->addChild(selectionButton = makeToggleButton(1, 1 + (17 * 2), store.editorUI, "Selection"));
			toolbarContainer->addChild(fillButton = makeToggleButton(1, 1 + (17 * 3), store.editorUI, "PaintBucket"));
			toolbarContainer->addChild(eyeDropperButton = makeToggleButton(1, 1 + (17 * 4), store.editorUI, "EyeDropper"));
			toolGroup = new ButtonGroup({ pencilButton, rubberButton, selectionButton, fillButton, eyeDropperButton });
			toolGroup->select(pencilButton);
		}

		PanelSelectorCreation: {
			panelContainer = new GameObject();
			const SpriteData* sprContainer = store.editorUI->getSpriteData("PanelContainer");
			panelContainer->addGraphic(sprContainer->makeSprite());
			panelContainer->setBoundingBox(sprContainer->width, sprContainer->height);
			panelContainer->setPosition((480 / 2) - (sprContainer->width / 2), 3.0f);
			panelContainer->setRelativeToView(false);
			addGameObject(panelContainer);

			panelContainer->addChild(designButton = makeToggleButton(1, 1, store.editorUI, "Design"));
			panelContainer->addChild(palleteButton = makeToggleButton(32, 1, store.editorUI, "Pallete"));
			panelGroup = new ButtonGroup({ designButton, palleteButton });
			panelGroup->select(designButton);
		}

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
		tilePallete->setBoundingBox(tilePallete->getBoundingBox().width * scale, tilePallete->getBoundingBox().height * scale);

		addGameObject(tilePallete);
		selectedTileSetTile = { 0, 0 };
	}

	Button* makeToggleButton(float x, float y, const SpriteSheet* _spriteSheet, std::string _prefix) {
		return new Button(x, y, _spriteSheet, _prefix + "Idle", _prefix + "Hover", _prefix + "Down", true);
	}

	std::vector<sf::Vector2i> fillTilemap(sf::Vector2i tileIndex) {

	}

	void setPanel(Panel panel) {
		switch (panel) {
			case Panel::Design:
				m_panel = Panel::Design;
				panelGroup->select(designButton);
				tilePallete->setVisible(false);
				map->setVisible(true);
				toolbarContainer->setVisible(true);
				break;
			case Panel::Pallete:
				m_panel = Panel::Pallete;
				panelGroup->select(palleteButton);
				tilePallete->setVisible(true);
				map->setVisible(false);
				toolbarContainer->setVisible(false);
				break;
		}
	}

	void update() {
		State::update();

		if (rightMouse->pressed()) {
			setPanel(Panel::Pallete);
		}
		
		if (rightMouse->released()) {
			setPanel(Panel::Design);
		}

		if (middleMouse->down()) {
			getCamera().setCenter(getCamera().getCenter() - getCamera().mapDistance(Input::GetMouseDelta()));
		};

		if (leftMouse->down()) {
			sf::Vector2f pos = getCamera().mapDistance(Input::GetMousePos());

			if (toolbarContainer->isVisible() && toolbarContainer->intersect(pos)) {
				tool = (Tool)toolGroup->getSelectedIndex();
			}
			else if (panelContainer->intersect(pos)) {
				if (designButton->isClicked()) {
					setPanel(Panel::Design);
				}
				else if (palleteButton->isClicked()) {
					setPanel(Panel::Pallete);
				}
			}
			else {
				sf::Vector2f projectedPos = getCamera().mapPixelToCoords(Input::GetMousePos()) / (float)map->getCellSize();

				if (m_panel == Panel::Pallete) {
					/*sf::Vector2f distPos = getCamera().mapDistance(Input::GetMousePos());

					if (tilePallete->intersect(distPos)) {
						selectedTileSetTile = tilePallete->getTileIndex(distPos);
						tilePallete->setSelectedPos(selectedTileSetTile);
					}*/
				} else {
					switch (tool) {
						case Tool::Pencil:
							for (int tileX = 0; tileX <= (tilePallete->endTile.x - tilePallete->startTile.x); tileX++) {
								for (int tileY = 0; tileY <= (tilePallete->endTile.y - tilePallete->startTile.y); tileY++) {
									if (map->inBounds(projectedPos.x + tileX, projectedPos.y + tileY)) {
										map->setTile(tilePallete->startTile.x + tileX, tilePallete->startTile.y + tileY, projectedPos.x + tileX, projectedPos.y + tileY, true);
									}
								}
							}
							break;
						case Tool::Rubber:
							map->deleteTile((int)projectedPos.x, (int)projectedPos.y);
							break;
						case Tool::Picker:
							if (map->inBounds((int)projectedPos.x, (int)projectedPos.y) && map->isTileSet((int)projectedPos.x, (int)projectedPos.y)) {
								this->selectedTileSetTile = map->getTileData((int)projectedPos.x, (int)projectedPos.y);
							};

							break;
					}
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
		map = new TileMap(16, 16, store.tileSet, 16, 16);

		for (int i = 0; i < 16; i++)
			map->setTile(0, 0, i, 2, true);

		for (int i = 8; i < 16; i++)
			map->setTile(0, 0, i, 1, true);

		player = new Player(map);

		//btn = new Button(50, 25, store.squareFont, "Test Button");

		addGameObject(map);
		addGameObject(player);
		//addGameObject(btn);
		camera = (Camera&)sf::View(sf::FloatRect(0, 0, 480, 270));
		camera.setFocused(player);
		setCamera(&camera);

		GameObject* object = new GameObject();
		object->setRelativeToView(false);
		sf::Sprite* spr = new sf::Sprite(*store.infoBar);
		object->addGraphic(spr);
		addGameObject(object);

		this->setClearColor(sf::Color(244, 104, 104, 255));
	}

};

class MainMenuState : public State {
private:

public:
	MainMenuState(const AssetStore& store, std::string name) : State(name) {
		Camera* camera = new Camera(480, 270);
		camera->setCenter(480 / 2, 270 / 2);
		setCamera(camera);


		GameObject* bg = new GameObject();
		bg->setGraphic(new sf::Sprite(*store.mainMenu));
		addGameObject(bg);
	}
};

void entry() {
	Game game = Game("Game Title", 960, 540);

	AssetStore store = AssetStore();

	//MainMenuState* mainMenuState = new MainMenuState(store, "MainMenu");
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