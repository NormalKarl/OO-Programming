#include "TileMap.h"

TileMap::TileMap(int mapWidth, int mapHeight, sf::Texture* texture, int cellSize, float tileSize)
	: GameObject(), mapWidth(mapWidth), mapHeight(mapHeight), texture(texture), cellSize(cellSize), tileSize(tileSize) {
	mapVA = sf::VertexArray(sf::PrimitiveType::Triangles, mapWidth * mapHeight * 6);
	collision = new bool[mapWidth * mapHeight];
	std::fill_n(collision, mapWidth * mapHeight, 0);

	gridVisible = false;
	gridVA = sf::VertexArray(sf::PrimitiveType::Lines, (mapWidth * 2) + (mapHeight * 2));

	for (int x = 0; x <= mapWidth; x++) {
		gridVA.append({ { (float)x * cellSize, 0 }, sf::Color::Black });
		gridVA.append({ { (float)x * cellSize, (float)mapHeight * cellSize }, sf::Color::Black });
	}

	for (int y = 0; y <= mapHeight; y++) {
		gridVA.append({ { 0, (float)y * cellSize }, sf::Color::Black });
		gridVA.append({ { (float)mapWidth * cellSize, (float)y * cellSize }, sf::Color::Black });
	}
}

TileMap::~TileMap() {
	delete[] collision;
}

void TileMap::setTile(int cellX, int cellY, int tileX, int tileY, bool solid) {
	if (!inBounds(tileX, tileY))
		return; // TODO add error message.
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


bool TileMap::isTileSet(int tileX, int tileY) {
	//return inBounds(tileX, tileY) && mapVA[((tileY * mapWidth) + tileX) * 6] == sf::Vertex();
	return false;
}

void TileMap::deleteTile(int tileX, int tileY) {
	if (!inBounds(tileX, tileY))
		return;

	for (int i = 0; i < 6; i++) {
		mapVA[((tileY * mapWidth) + tileX) * 6 + i] = sf::Vertex();
	}

	collision[tileY * mapWidth + tileX] = false;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (isVisible()) {
		states.texture = texture;
		states.transform *= getTransform();
		target.draw(mapVA, states);

		if (gridVisible)
			target.draw(gridVA, states);
	}
}

bool TileMap::isSolid(int tileX, int tileY) {
	return inBounds(tileX, tileY) ? collision[tileY * mapWidth + tileX] : false;
}
