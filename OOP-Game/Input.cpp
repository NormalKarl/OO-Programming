#include "Input.h"

InputState KeyRight = InputState::Up;
InputState KeyLeft = InputState::Up;
InputState KeyUp = InputState::Up;
InputState KeyDown = InputState::Up;
InputState KeyJump = InputState::Up;

bool InputDown(InputState& inputState) {
	return inputState == InputState::Pressed || inputState == InputState::Down;
}

bool InputPressed(InputState& inputState) {
	return inputState == InputState::Pressed;
}

void UpdateInput(InputState& inputState) {
	if(inputState == InputState::Pressed) {
		inputState = InputState::Down;
	} else if(inputState == InputState::Released) {
		inputState = InputState::Up;
	}
}
