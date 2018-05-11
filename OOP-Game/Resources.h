#pragma once

#include <vector>
#include <map>
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

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
class ResourceFactory;

struct SpriteData {
	friend class ResourceFactory;
	union {
		const SpriteSheet* spriteSheet;
		sf::Texture* m_texture;
	};
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

class ResourceFactory
{
private:
	struct ResourceEntry {
		std::string m_key;

		enum class EntryType {
			Texture,
			BitmapFont,
			Sprite,
			Music,
			Sound
		} m_type;

		union {
			sf::Texture* m_texture;
			BitmapFont* m_bitmapFont;
			SpriteData* m_sprites;
			sf::Music* m_music;
			sf::SoundBuffer* m_sound;
			void* m_ptr;
		};

		ResourceEntry(std::string _key, EntryType _type, sf::Texture* _texture) : m_key(_key), m_type(_type), m_texture(_texture) {}
		ResourceEntry(std::string _key, EntryType _type, BitmapFont* _bitmapFont) : m_key(_key), m_type(_type), m_bitmapFont(_bitmapFont) {}
		ResourceEntry(std::string _key, EntryType _type, SpriteData* _spriteData) : m_key(_key), m_type(_type), m_sprites(_spriteData) {}
		ResourceEntry(std::string _key, EntryType _type, sf::Music* _music) : m_key(_key), m_type(_type), m_music(_music) {}
		ResourceEntry(std::string _key, EntryType _type, sf::SoundBuffer* _sound) : m_key(_key), m_type(_type), m_sound(_sound) {}

		inline sf::Texture* getTexture() { return m_texture; }
		inline BitmapFont* getBitmapFont() { return m_bitmapFont; }
		inline SpriteData* getSprite() { return m_sprites; }
		inline sf::Music* getMusic() { return m_music; }
		inline sf::SoundBuffer* getSound() { return m_sound; }

		inline bool isTexture() { return m_type == EntryType::Texture; }
		inline bool isBitmapFont() { return m_type == EntryType::BitmapFont; }
		inline bool isSprite() { return m_type == EntryType::Sprite; }
		inline bool isMusic() { return m_type == EntryType::Music; }
		inline bool isSound() { return m_type == EntryType::Sound; }
	};

	std::vector<ResourceEntry*> m_resources;

	ResourceFactory() {}
	~ResourceFactory() {}

	ResourceFactory(const ResourceFactory& copy) {}
	void operator=(const ResourceFactory& value) {}

	bool isKeyValid(std::string _key);
	bool keyCheck(std::string _key);
	bool keyExists(std::string _key);
	ResourceEntry* find(std::string _key);

	template<typename T>
	T* check(std::string _key, ResourceEntry::EntryType _type) {
		ResourceEntry* entry = find(_key);

		if (entry != nullptr && entry->m_type == _type)
			return (T*)entry->m_ptr;

		return nullptr;
	}

	static ResourceFactory* s_instance;
public:

	//void loadDictionary(std::string _dictionaryPath);
	void loadTexture(std::string _key, std::string _texturePath);
	void loadBitmapFont(std::string _key, std::string _fontXML, std::string _pagePath);
	void loadSprite(std::string _key, std::string _textureKey, int _x, int _y, int _width, int _height, float _originX = 0.0f, float _originY = 0.0f);
	void loadSprite(std::string _key, std::string _textureKey, sf::IntRect _region, sf::Vector2f _origin = sf::Vector2f(0.0f, 0.0f));
	void loadMusic(std::string _key, std::string _musicPath);
	void loadSound(std::string _key, std::string _sfxPath);

	sf::Texture* getTexture(std::string _key);
	BitmapFont* getBitmapFont(std::string _key);
	SpriteData* getSprite(std::string _key);
	sf::Music* getMusic(std::string _key);
	sf::SoundBuffer* getSound(std::string _key);

	sf::Sprite* makeSFMLSprite(std::string _key);

	void clear();

	static ResourceFactory& getInstance();
};
