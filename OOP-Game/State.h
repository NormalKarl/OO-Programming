#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "GameObject.h"

class Game;

class Camera : public sf::View {
private:
	const GameObject* m_focused;

	int m_depth;
public:
	Camera() : sf::View() {}
	Camera(const GameObject* _focused) : sf::View(), m_focused(_focused) {}

	void update();

	inline const GameObject* getFocused() {
		return m_focused;
	}

	inline void setFocused(const GameObject* _focused) {
		m_focused = _focused;
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
public:
	State(std::string name = "");
	~State();
	virtual void update();
	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

	void addGameObject(GameObject* object);

	inline std::string getName() {
		return m_name;
	}

	inline Camera& getCamera() const {
		return *m_cameras[0];
	}

	void setCamera(Camera* camera);
};



