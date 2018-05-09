#pragma once

#include <vector>
#include <map>
#include <string>
#include <SFML\Graphics.hpp>

struct BitmapChar {
	unsigned int id;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	int xoffset;
	int yoffset;
	int xadvance;
	unsigned int page;
	unsigned int channel;

	float uvX;
	float uvY;
	float uvWidth;
	float uvHeight;
};

struct Kerning {
	unsigned int first;
	unsigned int second;
	int amount;
};

//Supports bitmap fonts created by AngleCode's BMFont software.
//Requires XML export.
class BitmapFont
{
private:
	BitmapChar chars[256];
	std::vector<Kerning> kerning[256];
	sf::Texture* texture;

	unsigned int lineHeight;
	unsigned int base;
	unsigned int scaleW;
	unsigned int scaleH;
	unsigned int pages;
	unsigned int size;
public:
	BitmapFont(const char* filename, std::vector<std::string> pagePaths);
	~BitmapFont();

	inline const std::vector<Kerning>* getKernings() const {
		return kerning;
	}

	inline const BitmapChar* getChars() const {
		return chars;
	}

	inline unsigned int getBase() const {
		return base;
	}

	inline unsigned int getLineHeight() const {
		return lineHeight;
	}

	inline sf::Texture* getTexture() const {
		return texture;
	}

	int getWidth(const std::string& text);
};

class SpriteSheet;

struct SpriteData {
	const SpriteSheet* spriteSheet;
	std::string name;

	//Very clever indeed
	union {
		struct {
			int x;
			int y;
			int width;
			int height;
		};

		sf::IntRect rect;
	};

	union {
		struct {
			float originX;
			float originY;
		};

		sf::Vector2f origin;
	};

	SpriteData() : x(0), y(0), width(0), height(0), originX(0.0f), originY(0.0f) { }
	sf::Sprite* makeSprite() const;
};

class SpriteSheet {
private:
	sf::Texture* texture;
	std::vector<SpriteData> m_sprites;
public:
	SpriteSheet(const char* xmlSheet);
	virtual ~SpriteSheet();

	inline const sf::Texture& getTexture() const {
		return *texture;
	}

	const SpriteData* getSpriteData(std::string name) const;
	sf::Sprite* makeSprite(std::string name) const;
};

class ResourceManager
{
private:
	std::map<std::string, void*> m_resources;
public:
	static ResourceManager* s_resourceManager;
	ResourceManager();
	~ResourceManager();

	static void LoadTexture(std::string _key, std::string path);
};
