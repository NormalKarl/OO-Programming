#pragma once

#include <string>
#include "Engine.h"

class Button : public GameObject
{
private:
	Label m_label;
	sf::RectangleShape shape;
public:
	Button(float x, float y, const BitmapFont* font, std::string text);
	~Button();
	void update();
};

