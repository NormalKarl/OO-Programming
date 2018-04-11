#include "Button.h"

Button::Button(float x, float y, const BitmapFont* font, std::string text)
{
	buttonType = LabelButton;
	sf::Vector2f padding = { 10, 10 };
	setPosition(x, y);
	m_label = Label(font, text);
	m_label.setPosition(padding);
	sf::Vector2f size = { m_label.getLabelWidth() + (padding.x * 2), m_label.getLabelHeight() + (padding.y * 2) };
	shape = sf::RectangleShape(size);

	setBoundingBox(size.x, size.y);

	shape.setFillColor(sf::Color::Green);

	addGraphic(&shape);
	addGraphic(&m_label);

	setRelativeToView(false);
}

Button::Button(float x, float y, const SpriteSheet* sheet, std::string idle, std::string hover, std::string down, bool toggle)
	: Button(x, y, sheet->getSpriteData(idle), sheet->getSpriteData(hover), sheet->getSpriteData(down), toggle){

}

Button::Button(float x, float y, const SpriteData* idle, const SpriteData* hover, const SpriteData* down, bool toggle) {
	setPosition(x, y);
	buttonType = TextureButton;
	state = None;

	this->toggle = toggle;

	idleSprite = idle->makeSprite();
	hoverSprite = hover->makeSprite();
	downSprite = down->makeSprite();

	setBoundingBox((float)idle->width, (float)idle->height);

	setGraphic(idleSprite);
	setRelativeToView(false);

	toggled = false;

	buttonGroup = nullptr;
}


Button::~Button()
{
}

void Button::update() {
	bool i = intersect(getState()->getCamera().mapDistance(Input::GetMousePos()));

	if (i) {
		if (Input::GetState(sf::Mouse::Button::Left)->pressed()) {
			setGraphic(downSprite);
			state = Clicked;

			if (buttonGroup != nullptr && toggle && !toggled) {
				toggled = !toggled;
			}
			else {
				toggled = !toggled;
			}

			if (buttonGroup != nullptr) {
				buttonGroup->select(this);
			}
		}
		else if (Input::GetState(sf::Mouse::Button::Left)->down()) {
			setGraphic(downSprite);
			state = Helt;
		}
		else {
			setGraphic(hoverSprite);
			state = Hover;
		}
	}
	else {
		state = None;
		setGraphic(idleSprite);
	}

	if (toggle && toggled) {
		setGraphic(downSprite);
	}
}

///ButtonGroup

ButtonGroup::ButtonGroup() {
	selectedIndex = -1;
}

ButtonGroup::ButtonGroup(std::vector<Button*> _buttons) {
	selectedIndex = -1;
	m_buttons = _buttons;

	for (Button* button : m_buttons) {
		if (button != nullptr) {
			button->setButtonGroup(this);
		}
	}
}

void ButtonGroup::addButton(Button* _button) {
	m_buttons.push_back(_button);
	_button->setButtonGroup(this);
}

void ButtonGroup::select(Button* selectButton) {
	int i = 0;

	for (Button* button : m_buttons) {
		if (button == selectButton) {
			selectButton->setToggled(true);
			selectedIndex = i;
		}
		else {
			button->setToggled(false);
		}

		i++;
	}
}

Button* ButtonGroup::getSelected() {
	if (selectedIndex != -1) {
		return m_buttons[selectedIndex];
	}
	else {
		return nullptr;
	}
}