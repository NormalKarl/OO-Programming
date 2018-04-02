#include "TileMap.h"

TileMap::TileMap(int mapWidth, int mapHeight, sf::Texture* texture, int cellSize, float tileSize)
	: GameObject(), mapWidth(mapWidth), mapHeight(mapHeight), texture(texture), cellSize(cellSize), tileSize(tileSize) {
	mapVA = sf::VertexArray(sf::PrimitiveType::Triangles, mapWidth * mapHeight * 6);
	collision = new bool[mapWidth * mapHeight];
	std::fill_n(collision, mapWidth * mapHeight, 0);
}

TileMap::~TileMap() {
	delete[] collision;
}

void TileMap::setTile(int cellX, int cellY, int tileX, int tileY, bool solid) {
	//First Triangle
	mapVA[((tileY * mapWidth) + tileX) * 6] = sf::Vertex{ { (float)(tileX * tileSize), (float)(tileY * tileSize) }, sf::Color::White,{ (float)(cellX * cellSize), (float)(cellY * cellSize) } };
	mapVA[((tileY * mapWidth) + tileX) * 6 + 1] = sf::Vertex{ { (float)(tileX * tileSize) + tileSize, (float)tileY * tileSize }, sf::Color::White,{ (float)(cellX * cellSize + cellSize), (float)(cellY * cellSize) } };
	mapVA[((tileY * mapWidth) + tileX) * 6 + 2] = sf::Vertex{ { (float)tileX * tileSize, (float)(tileY * tileSize) + tileSize }, sf::Color::White,{ (float)(cellX * cellSize), (float)(cellY * cellSize + cellSize) } };

	//Second Triangle
	mapVA[((tileY * mapWidth) + tileX) * 6 + 3] = sf::Vertex{ { (float)tileX * tileSize, (float)(tileY * tileSize) + tileSize }, sf::Color::White,{ (float)(cellX * cellSize), (float)(cellY * cellSize + cellSize) } };
	mapVA[((tileY * mapWidth) + tileX) * 6 + 4] = sf::Vertex{ { (float)(tileX * tileSize) + tileSize, (float)(tileY * tileSize) }, sf::Color::White,{ (float)(cellX * cellSize + cellSize), (float)(cellY * cellSize) } };
	mapVA[((tileY * mapWidth) + tileX) * 6 + 5] = sf::Vertex{ { (float)(tileX * tileSize) + tileSize, (float)(tileY * tileSize) + tileSize }, sf::Color::White,{ (float)(cellX * cellSize), (float)(cellY * cellSize + cellSize) } };

	collision[tileY * mapWidth + tileX] = solid;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = texture;
	target.draw(mapVA, states);
}

bool TileMap::isSolid(int tileX, int tileY) {
	return inBounds(tileX, tileY) ? collision[tileY * mapWidth + tileX] : false;
}
