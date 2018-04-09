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

	inline void setState(State _state) {
		m_state = _state;
	}

	inline bool InputState::pressed() const {
		return m_state == State::Pressed;
	}

	inline bool InputState::down() const {
		return m_state == State::Pressed || m_state == State::Down;
	}

	inline bool InputState::released() const {
		return m_state == State::Released;
	}

	inline bool InputState::up() const {
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
	sf::Vector2i mousePos;
	sf::Vector2i mouseDelta;
	int mouseWheel;
	int mouseWheelDelta;
public:
	Input(const Game* _parent);
	~Input() {}
	void clear();
	void updateStates();
	void event(sf::Event event);

	inline const InputState* Input::getState(sf::Keyboard::Key key) {
		return &m_keyStates[key];
	}

	inline const InputState* Input::getState(sf::Mouse::Button button) {
		return &m_buttonStates[button];
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

	inline static Input& GetInput() {
		return *Input::s_input;
	}

	inline static const InputState* Input::GetState(sf::Keyboard::Key key) {
		return &(GetInput().m_keyStates[key]);
	}

	inline static const InputState* Input::GetState(sf::Mouse::Button button) {
		return &(GetInput().m_buttonStates[button]);
	}

	inline static sf::Vector2i GetMousePos() {
		return s_input->mousePos;
	}

	inline static sf::Vector2i GetMouseDelta() {
		return s_input->mouseDelta;
	}

	inline static int GetMouseX() {
		return s_input->mousePos.x;
	}

	inline static int GetMouseY() {
		return s_input->mousePos.y;
	}
};
