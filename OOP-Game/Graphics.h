#pragma once

#include <SFMl/Graphics.hpp>

class BitmapFont;

class Graphic : public sf::Transformable, public sf::Drawable {
public:
	virtual void update() {}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class Animation : public Graphic {
private:
	const sf::Texture* texture;
	std::vector<sf::Sprite> m_frames;
	float m_speed;
	float m_elapsed;
	bool m_playing;
public:
	Animation(const sf::Texture* texture, std::vector<sf::IntRect> frames, float speed);
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Label : public Graphic {
private:
	const BitmapFont* font;
	sf::VertexArray va;
	std::string m_text;
	sf::Vector2f m_labelSize;
public:
	Label() {}
	Label(const BitmapFont* font, std::string text);
	virtual ~Label();

	inline std::string getText() const { return m_text; }
	void setText(std::string text);

	inline float getLabelWidth() { return m_labelSize.x; }
	inline float getLabelHeight() { return m_labelSize.y; }
	inline sf::Vector2f getLabelSize() { return m_labelSize; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
