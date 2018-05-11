#pragma once

#include <SFML\Graphics.hpp>
#include <map>

class State;
class Input;

/*
Game is a Singleton accessed through getInstance().
Here you can set the title, size and state of the current window,
as well as add and set states, and start and stop the main game loop.
*/
class Game
{
private:
	static Game* s_instance;

	//Window Objects
	sf::RenderWindow* m_window;
	sf::VideoMode m_videoMode;
	std::string m_title;
	//States
	std::vector<State*> m_states;
	State* m_state;
	bool m_running;

	Game();
	~Game();
	Game(const Game& copy) {}
	void operator=(const Game& value) {}
public:

	inline std::string getTitle() { return m_title; }
	inline void setTitle(std::string _title) { m_title = _title; }
	inline void setSize(unsigned int _width, unsigned int _height) { m_videoMode = sf::VideoMode(_width, _height); }

	void start();

	inline State* getState() { return m_state; }
	inline const sf::View& getView() { return m_window->getDefaultView(); }
	inline sf::Vector2u getSize() { return { m_videoMode.width, m_videoMode.height }; }
	inline sf::RenderWindow* getWindow() { return m_window; }

	bool hasState(State* state);
	bool hasState(std::string name);
	void addState(State* state);
	void setState(State* state);
	void setState(std::string name);
	void removeState(State* state);
	void removeState(std::string name);
	float getDelta();

	static Game& getInstance();
};

namespace IO {
	void Error(std::string _errorMessage);
}