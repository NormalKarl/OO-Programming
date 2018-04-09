#pragma once

#include <string>
#include "Engine.h"

enum ButtonType {
	LabelButton,
	TextureButton
};

class Button : public GameObject
{
private:
	ButtonType buttonType;

	union {
		struct {
			Label m_label;
			sf::RectangleShape shape;
		};

		struct {
			const SpriteData* texture;
			sf::Sprite* spr;
		};
	};

public:
	Button(float x, float y, const BitmapFont* font, std::string text);
	Button(const SpriteData* texture);
	~Button();
	void update();
};

