#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.h"

class TileMap : public GameObject {
private:
	sf::Texture* texture;
	int cellSize;

	sf::VertexArray mapVA;
	int mapWidth;
	int mapHeight;
	float tileSize;

	bool* collision;
public:
	TileMap() {}
	TileMap(int mapWidth, int mapHeight, sf::Texture* texture, int cellSize, float tileSize = 16.0f);
	~TileMap();

	void setTile(int cellX, int cellY, int tileX, int tileY, bool solid);
	bool isSolid(int tileX, int tileY);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	inline int getMapWidth() {
		return mapWidth;
	}

	inline int getMapHeight() {
		return mapHeight;
	}

	inline bool inBounds(int tileX, int tileY) {
		return tileX >= 0 && tileY >= 0 && tileX < mapWidth && tileY < mapHeight;
	}

	inline sf::FloatRect TileMap::getTileBounds(int tileX, int tileY) {
		return { tileX * tileSize, tileY * tileSize, tileSize, tileSize };
	}

	inline float getTileSize() {
		return tileSize;
	}
};
