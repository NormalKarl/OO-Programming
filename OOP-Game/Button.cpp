#include "Button.h"

Button::Button(float x, float y, const BitmapFont* font, std::string text)
{
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


Button::~Button()
{
}

void Button::update() {
	if (intersect(Input::GetMousePos())) {
		if (Input::GetState(sf::Mouse::Button::Left)->pressed()) {
			shape.setFillColor(sf::Color::Yellow);
		} else if (Input::GetState(sf::Mouse::Button::Left)->down()) {
			shape.setFillColor(sf::Color::Magenta);
		} else {
			shape.setFillColor(sf::Color::Blue);
		}
	} else {

		shape.setFillColor(sf::Color::Green);
	}
}
