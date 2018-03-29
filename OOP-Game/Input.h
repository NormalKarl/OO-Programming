#pragma once

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

enum class InputState
{
	Pressed,
	Down,
	Released,
	Up,
};

class Input {
private:
	InputState keyStates[sf::Keyboard::KeyCount];
	InputState buttonStates[sf::Mouse::ButtonCount];
public:

};

extern InputState KeyRight;
extern InputState KeyLeft;
extern InputState KeyUp;
extern InputState KeyDown;
extern InputState KeyJump;

bool InputDown(InputState& inputState);

bool InputPressed(InputState& inputState);

void UpdateInput(InputState& inputState);