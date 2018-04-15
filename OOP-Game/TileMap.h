#pragma once

#include <SFML/Graphics.hpp>

#include "GameObject.h"

class TileMap : public GameObject {
private:
	sf::Texture* texture;
	int cellSize;

	sf::VertexArray mapVA;
	sf::Vector2i* tileData;

	int mapWidth;
	int mapHeight;
	float tileSize;

	bool* collision;

	bool gridVisible;
	sf::VertexArray gridVA;
	
public:
	TileMap() {}
	TileMap(int mapWidth, int mapHeight, sf::Texture* texture, int cellSize, float tileSize = 16.0f);
	~TileMap();

	void setTile(int cellX, int cellY, int tileX, int tileY, bool solid);
	bool isTileSet(int tileX, int tileY);
	bool isSolid(int tileX, int tileY);
	void deleteTile(int tileX, int tileY);
	sf::Vector2i getTileData(int tileX, int tileY);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	inline int getMapWidth() {
		return mapWidth;
	}

	inline int getMapHeight() {
		return mapHeight;
	}

	inline int getCellSize() {
		return cellSize;
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

	inline bool isGridVisible() {
		return gridVisible;
	}

	inline void setGridVisible(bool gridVisible) {
		this->gridVisible = gridVisible;
	}
};
