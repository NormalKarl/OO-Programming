#include "Input.h"
#include "Game.h"

Input* Input::s_input = NULL;

void InputState::update() {
	if (pressed()) {
		setState(State::Down);
	} else if (released()) {
		setState(State::Up);
	}
}

Input::Input(const Game* _parent) : m_parent(_parent) {
	s_input = this;
	clear();
}

void Input::clear() {
	std::fill_n(m_keyStates, sf::Keyboard::KeyCount, InputState::State::Up);
	std::fill_n(m_buttonStates, sf::Mouse::ButtonCount, InputState::State::Up);
	mouseWheel = 0;
	mouseWheelDelta = 0;
}

void Input::updateStates() {
	for (InputState& keyState : m_keyStates) {
		keyState.update();
	}

	for (InputState& buttonState : m_buttonStates) {
		buttonState.update();
	}

	mouseDelta = { 0, 0 };
}

void Input::event(sf::Event e) {
	switch (e.type) {
		case sf::Event::EventType::KeyPressed:
			m_keyStates[e.key.code].setState(InputState::State::Pressed);
			break;
		case sf::Event::EventType::KeyReleased:
			m_keyStates[e.key.code].setState(InputState::State::Released);
			break;
		case sf::Event::EventType::MouseButtonPressed:
			m_buttonStates[e.mouseButton.button].setState(InputState::State::Pressed);
			break;
		case sf::Event::EventType::MouseButtonReleased:
			m_buttonStates[e.mouseButton.button].setState(InputState::State::Released);
			break;
		case sf::Event::EventType::MouseMoved:
			sf::Vector2i tempMousePos = mousePos;
			mousePos = { e.mouseMove.x, e.mouseMove.y };
			mouseDelta = mousePos - tempMousePos;
			break;
	}
}
