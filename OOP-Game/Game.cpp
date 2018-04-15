#include <algorithm>
#include "Game.h"
#include "Input.h"
#include "State.h"

Game* Game::s_game = NULL;

Game::Game(std::string _title, unsigned int _width, unsigned int _height) {
	m_title = _title;
	m_videoMode = sf::VideoMode(_width, _height);
	m_running = false;
	m_input = new Input(this);
	s_game = this;

	m_state = NULL;
}

Game::~Game() {
	delete m_window;
	delete m_input;
}

void Game::update() {
	if (m_state != NULL)
		m_state->update();
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (m_state != NULL)
		target.draw(*m_state, states);
}

void Game::start() {
	m_window = new sf::RenderWindow(m_videoMode, m_title, sf::Style::None);
	m_window->setFramerateLimit(60);
	m_window->setVerticalSyncEnabled(true);
	m_window->setKeyRepeatEnabled(false);
	m_running = true;
	m_input->clear();

	while (m_running) {
		m_input->updateStates();

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
					m_input->event(e);
					break;
				/*case sf::Event::TextEntered:
					printf("%c", e.text.unicode);
					break;*/
			}
		}

		update();
		m_window->clear();
		m_window->draw(*this);
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
		state->m_game = this;

		if (m_state == NULL) {
			m_state = state;
			printf("Info: Current state has not been set. Setting using parsed State.\n");
		}

		return;
	}

	printf("Error: State already exists.\n");
}

void Game::setState(State* state) {
	if (!hasState(state)) {
		printf("Warning: State doesn't already exist in Game. Adding...\n");
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

	printf("Error: Couldn't find state with the name '%s'\n", name.c_str());
}

void Game::removeState(State* state) {
	for (std::vector<State*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		if (*it == state) {
			m_states.erase(it);
			return;
		}
	}

	printf("Error: Couldn't remove state.\n");
}

void Game::removeState(std::string name) {
	for (std::vector<State*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		if ((*it)->getName() == name) {
			m_states.erase(it);
			return;
		}
	}

	printf("Error: Couldn't remove state with the name '%s'\n", name.c_str());
}

float Game::getDelta() {
	return 60.0f / 1000.0f;
}