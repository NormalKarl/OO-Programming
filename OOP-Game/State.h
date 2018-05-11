#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "GameObject.h"

class Game;

class State : public sf::Drawable
{
private:
	friend class Game;

	std::string m_name;
	sf::View m_camera;
	sf::Color m_backgroundColour;
	std::vector<GameObject*> m_gameObjects;


	bool m_pollOrder;
	void pollOrder();
public:
	State(std::string name = "");
	virtual ~State();
	virtual void update();
	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;
	void reorder();

	sf::Vector2f map(sf::Vector2i _pixel);

	void add(GameObject* object);

	inline std::string getName() { return m_name; }
	inline sf::View& getCamera() { return m_camera; }
	inline void setCamera(const sf::View& _camera) { this->m_camera = _camera; }
	inline sf::Color getBackgroundColour() { return m_backgroundColour; }
	inline void setBackgroundColour(const sf::Color& _backgroundColour) { m_backgroundColour = _backgroundColour; }
};



