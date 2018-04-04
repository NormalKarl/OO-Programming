#pragma once

#include <SFML\Graphics.hpp>

class State;
class Input;

class Game : public sf::Drawable
{
private:
	static Game* s_game;

	//Window Objects
	sf::RenderWindow* m_window;
	sf::VideoMode m_videoMode;
	std::string m_title;
	Input* m_input;
	//States
	std::vector<State*> m_states;
	State* m_state;
	
	bool m_running;
public:
	Game(std::string _title, unsigned int _width, unsigned int _height);
	~Game();

	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	void start();

	inline State* getState() {
		return m_state;
	}

	inline const sf::View& getView() {
		return m_window->getDefaultView();
	}

	bool hasState(State* state);
	bool hasState(std::string name);
	void addState(State* state);
	void setState(State* state);
	void setState(std::string name);
	void removeState(State* state);
	void removeState(std::string name);
	float getDelta();

	static Game* getGame() {
		return s_game;
	}
};

