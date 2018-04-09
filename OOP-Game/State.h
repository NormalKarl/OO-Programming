#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "GameObject.h"

class Game;

class Camera : public sf::View {
private:
	friend class State;
	State* m_parent;
	const GameObject* m_focused;

	int m_depth;
public:
	Camera() : sf::View() {}
	Camera(float width, float height) : sf::View({width / 2, height / 2}, { width, height }) {}
	Camera(const GameObject* _focused) : sf::View(), m_focused(_focused) {}

	void update();

	inline const GameObject* getFocused() {
		return m_focused;
	}

	inline void setFocused(const GameObject* _focused) {
		m_focused = _focused;
	}

	sf::Vector2f mapPixelToCoords(sf::Vector2i mousePos);

	inline int getDepth() const {
		return m_depth;
	}
};

class State : public sf::Drawable
{
private:
	friend class Game;
	Game* m_game;

	std::string m_name;
	std::vector<GameObject*> m_gameObjects;
	std::vector<Camera*> m_cameras;

	sf::Color m_clearColor;

	bool m_pollOrder;
	void pollOrder();
public:
	State(std::string name = "");
	virtual ~State();
	virtual void update();
	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

	void addGameObject(GameObject* object);

	inline Game* getGame() {
		return m_game;
	}

	inline std::string getName() {
		return m_name;
	}

	inline Camera& getCamera() const {
		return *m_cameras[0];
	}

	inline sf::Color getClearColor() {
		return m_clearColor;
	}

	inline void setClearColor(const sf::Color& clearColor) {
		m_clearColor = clearColor;
	}

	void setCamera (Camera* camera);
	void reorder();
};



