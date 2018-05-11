#include <algorithm>
#include "Game.h"
#include "Input.h"
#include "State.h"

Game* Game::s_instance = nullptr;

Game::Game() {
	m_title = "Game Window";
	m_videoMode = sf::VideoMode(640, 480);
	m_running = false;
	s_instance = this;

	m_state = nullptr;
}

Game::~Game() {
	delete m_window;
}

void Game::start() {
	m_window = new sf::RenderWindow(m_videoMode, m_title/*, sf::Style::None*/);
	m_window->setFramerateLimit(60);
	m_window->setVerticalSyncEnabled(true);
	m_window->setKeyRepeatEnabled(false);
	m_running = true;

	Input& input = Input::getInstance();
	input.clear();

	while (m_running) {
		input.updateStates();

		sf::Event e;

		while (m_window->pollEvent(e)) {
			switch (e.type) {
				case sf::Event::Closed:
					m_running = false;
					break;
				//Parse of all of the Input related events to Input.
				case sf::Event::KeyPressed:
				case sf::Event::KeyReleased:
				case sf::Event::MouseButtonPressed:
				case sf::Event::MouseButtonReleased:
				case sf::Event::MouseWheelScrolled:
				case sf::Event::MouseMoved:
				case sf::Event::MouseEntered:
				case sf::Event::MouseLeft:
					input.event(e);
					break;
				/*case sf::Event::TextEntered:
					printf("%c", e.text.unicode);
					break;*/
			}
		}

		if (m_state != nullptr)
			m_state->update();

		m_window->clear();

		if (m_state != nullptr)
			m_window->draw(*m_state);

		m_window->display();

		if (!m_running) {
			m_window->close();
		}
	}
}

bool Game::hasState(State* state) {
	return std::find(m_states.begin(), m_states.end(), state) != m_states.end();
}

bool Game::hasState(std::string name) {
	for (State* state : m_states) {
		if (state->getName() == name)
			return true;
	}

	return false;
}

void Game::addState(State* state) {
	if (!hasState(state)) {
		m_states.push_back(state);

		if (m_state == nullptr) {
			m_state = state;
		}

		return;
	}

	IO::Error("State already exists.");
}

void Game::setState(State* state) {
	if (!hasState(state)) {
		m_states.push_back(state);
	}

	m_state = state;
}

void Game::setState(std::string name) {
	for (State* state : m_states) {
		if (state->getName() == name) {
			m_state = state;
			return;
		}
	}

	IO::Error("Couldn't find state with the name '" + name + "'");
}

void Game::removeState(State* state) {
	for (std::vector<State*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		if (*it == state) {
			m_states.erase(it);
			return;
		}
	}

	IO::Error("Couldn't remove state.");
}

void Game::removeState(std::string name) {
	for (std::vector<State*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		if ((*it)->getName() == name) {
			m_states.erase(it);
			return;
		}
	}

	IO::Error("Couldn't remove state with the name '" + name + "'");
}

float Game::getDelta() {
	return 1.0f / 60.0f;
}


void IO::Error(std::string _errorMessage) {
	printf(("Error: " + _errorMessage + "\n").c_str());
}

Game& Game::getInstance() {
	if (s_instance == nullptr)
		s_instance = new Game();

	return *s_instance;
}