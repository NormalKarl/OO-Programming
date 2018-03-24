#pragma once

enum class InputState
{
	Pressed,
	Down,
	Released,
	Up,
};

extern InputState KeyRight;
extern InputState KeyLeft;
extern InputState KeyUp;
extern InputState KeyDown;
extern InputState KeyJump;

bool InputDown(InputState& inputState);

bool InputPressed(InputState& inputState);

void UpdateInput(InputState& inputState);