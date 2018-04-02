#pragma once

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

class Game;


class InputState {
public:
	enum class State
	{
		Pressed,
		Down,
		Released,
		Up,
	};
private:
	State m_state;
public:
	InputState() : m_state(State::Up) { }
	InputState(State initalState) : m_state(initalState) { }
	void update();

	inline State getState() {
		return m_state;
	}

	inline void setState(State state) {
		m_state = state;
	}

	inline bool InputState::pressed() {
		return m_state == State::Pressed;
	}

	inline bool InputState::down() {
		return m_state == State::Pressed || m_state == State::Down;
	}

	inline bool InputState::released() {
		return m_state == State::Released;
	}

	inline bool InputState::up() {
		return m_state == State::Released || m_state == State::Up;
	}

	bool operator== (const State& rhs) const {
		return m_state == rhs;
	}

	bool operator!= (const State& rhs) const {
		return m_state != rhs;
	}
};

class Input {
private:
	static Input* s_input;

	const Game* m_parent;
	InputState m_keyStates[sf::Keyboard::KeyCount];
	InputState m_buttonStates[sf::Mouse::ButtonCount];
	int mouseX;
	int mouseY;
	int mouseXDelta;
	int mouseYDelta;
	int mouseWheel;
	int mouseWheelDelta;
public:
	Input(const Game* _parent);
	~Input() {}
	void clear();
	void updateStates();
	void event(sf::Event event);

	inline InputState& Input::getState(sf::Keyboard::Key key) {
		return m_keyStates[key];
	}

	inline InputState& Input::getState(sf::Mouse::Button button) {
		return m_buttonStates[button];
	}

	inline bool pressed(sf::Keyboard::Key key) {
		return m_keyStates[key].pressed();
	}

	inline bool down(sf::Keyboard::Key key) {
		return m_keyStates[key].down();
	}

	inline bool released(sf::Keyboard::Key key) {
		return m_keyStates[key].released();
	}

	inline bool up(sf::Keyboard::Key key) {
		return m_keyStates[key].up();
	}

	inline bool pressed(sf::Mouse::Button button) {
		return m_buttonStates[button].pressed();
	}

	inline bool down(sf::Mouse::Button button) {
		return m_buttonStates[button].down();
	}

	inline bool released(sf::Mouse::Button button) {
		return m_buttonStates[button].released();
	}

	inline bool up(sf::Mouse::Button button) {
		return m_buttonStates[button].up();
	}

	inline static Input& getInput() {
		return *Input::s_input;
	}
};
