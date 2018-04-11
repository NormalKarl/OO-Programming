#pragma once

#include <string>
#include "Engine.h"

class ButtonGroup;

enum ButtonType {
	LabelButton,
	TextureButton
};

enum ButtonState {
	None,
	Hover,
	Helt,
	Clicked,
};

class Button : public GameObject
{
private:
	ButtonType buttonType;
	sf::RectangleShape shape;

	ButtonState state;
	bool toggle;
	bool toggled;

	ButtonGroup* buttonGroup;

	union {
		struct {
			Label m_label;
		};

		struct {
			const SpriteData* texture;
			sf::Sprite* idleSprite;
			sf::Sprite* hoverSprite;
			sf::Sprite* downSprite;
			sf::Color selectedColour;
		};
	};

public:
	Button(float x, float y, const BitmapFont* font, std::string text);
	Button(float x, float y, const SpriteSheet* sheet, std::string idle, std::string hover, std::string down, bool toggle = false);
	Button(float x, float y, const SpriteData* idle, const SpriteData* hover, const SpriteData* down, bool toggle = false);
	~Button();
	void update();

	inline bool isClicked() {
		return state == Clicked;
	}

	inline void setToggled(bool toggled) {
		this->toggled = toggled;
	}

	inline bool isToggled() {
		return toggled;
	}

	inline void setButtonGroup(ButtonGroup* group) {
		this->buttonGroup = group;
	}

};

class ButtonGroup {
private:
	int selectedIndex;
	std::vector<Button*> m_buttons;
public:
	ButtonGroup();
	ButtonGroup(std::vector<Button*> _buttons);
	virtual ~ButtonGroup() { }

	void addButton(Button* _button);

	inline int getSelectedIndex() {
		return selectedIndex;
	}

	void select(Button* button);
	Button* getSelected();

};
