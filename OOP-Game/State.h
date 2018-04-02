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
public:
	Camera(const GameObject* _focused) : sf::View(), m_focused(_focused) {}
};

class State : public sf::Drawable
{
private:
	friend class Game;
	Game* m_game;

	std::string m_name;
	std::vector<GameObject*> m_gameObjects;
	Camera m_camera;
public:
	State(std::string name = "");
	~State();
	void update();
	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

	void addGameObject(GameObject* object);

	inline std::string getName() {
		return m_name;
	}

	inline Camera& getCamera() {
		return m_camera;
	}

	inline void setCamera(const Camera& _camera) {
		this->m_camera = _camera;
	}
};



