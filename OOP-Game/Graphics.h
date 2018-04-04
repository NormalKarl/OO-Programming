#pragma once

#include <SFMl/Graphics.hpp>

class Updatable {
public:
	virtual void update() = 0;
};

class Graphic : public sf::Transformable, public sf::Drawable, public Updatable {
public:
	virtual void update() = 0;
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

/*class TileMap : public sf::Transformable, public sf::Drawable {
private:
	const sf::Texture* texture;
	sf::VertexArray mapVA;
	int mapWidth;
	int mapHeight;
public:
	TileMap(const sf::Texture* texture);
	~TileMap();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};*/