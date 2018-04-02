#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include "GameObject.h"

class Game;

class Camera : public sf::View {

};

class State : public sf::Drawable
{
private:
	friend class Game;
	Game* m_game;

	std::string m_name;
	std::vector<GameObject> m_gameObjects;
	Camera m_camera;
public:
	State(std::string name);
	~State();
	void update();
	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

	inline std::string getName() {
		return m_name;
	}
};



