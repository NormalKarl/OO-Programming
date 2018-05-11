#include "Resources.h"

#include <fstream>
#include <sstream>
#include <string>

#include "tinyxml2.h"
#include "Game.h"

BitmapFont::BitmapFont(const char* filename, std::vector<std::string> pagePaths) {
	//Fill chars array with empty BitmapChar objects.
	std::fill_n((char*)chars, sizeof(BitmapChar) * 256, NULL);

	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);

	tinyxml2::XMLElement* commonEle = doc.FirstChildElement("font")->FirstChildElement("common");
	lineHeight = commonEle->IntAttribute("lineHeight");
	base = commonEle->IntAttribute("base");
	scaleW = commonEle->IntAttribute("scaleW");
	scaleH = commonEle->IntAttribute("scaleH");
	pages = commonEle->IntAttribute("pages");

	tinyxml2::XMLElement* e = doc.FirstChildElement("font")->FirstChildElement("chars");
	texture = new sf::Texture();
	texture->loadFromFile(pagePaths[0]);

	for (tinyxml2::XMLElement* c = (tinyxml2::XMLElement*)e->FirstChild(); c != NULL; c = (tinyxml2::XMLElement*)c->NextSibling()) {
		BitmapChar ch = { 0 };

		ch.id = c->IntAttribute("id");
		ch.x = c->IntAttribute("x");
		ch.y = c->IntAttribute("y");
		ch.width = c->IntAttribute("width");
		ch.height = c->IntAttribute("height");
		ch.xoffset = c->IntAttribute("xoffset");
		ch.yoffset = c->IntAttribute("yoffset");
		ch.xadvance = c->IntAttribute("xadvance");
		ch.page = c->IntAttribute("page");
		ch.channel = c->IntAttribute("chnl");

		//Transform the x, y, width and height of the sprite into normalized positions.
		//This is used for the Rasterizer class.
		ch.uvX = (float)ch.x / (float)texture->getSize().x;
		ch.uvY = (float)ch.y / (float)texture->getSize().y;
		ch.uvWidth = (float)ch.width / (float)texture->getSize().x;
		ch.uvHeight = (float)ch.height / (float)texture->getSize().y;

		chars[ch.id] = ch;
	}


	tinyxml2::XMLElement* kerningsElement = doc.FirstChildElement("font")->FirstChildElement("kernings");

	if (kerningsElement != NULL) {
		for (tinyxml2::XMLElement* c = (tinyxml2::XMLElement*)kerningsElement->FirstChild(); c != NULL; c = (tinyxml2::XMLElement*)c->NextSibling()) {
			Kerning k = { 0 };

			k.first = c->IntAttribute("first");
			k.second = c->IntAttribute("second");
			k.amount = c->IntAttribute("amount");

			kerning[k.second].push_back(k);
		}
	}

	return;
}


BitmapFont::~BitmapFont()
{
}


int BitmapFont::getWidth(const std::string& text) {
	int currentX = 0;

	for (int c = 0; c < (int)text.size(); c++) {
		BitmapChar bc = chars[text[c]];

		currentX += bc.xadvance;
	}

	return currentX;
}


sf::Sprite* SpriteData::makeSprite() const {
	sf::Sprite* sprite = new sf::Sprite();
	sprite->setTexture(spriteSheet->getTexture());
	sprite->setTextureRect(rect);
	sprite->setOrigin(origin);
	return sprite;
}

SpriteSheet::SpriteSheet(const char* xmlSheet) {
	using namespace tinyxml2;
	XMLDocument doc;
	doc.LoadFile(xmlSheet);

	XMLElement* mainEle = doc.FirstChildElement("SpriteSheet");

	texture = new sf::Texture();
	texture->loadFromFile(mainEle->Attribute("Source"));

	for (XMLElement* c = (XMLElement*)mainEle->FirstChild(); c != NULL; c = (XMLElement*)c->NextSibling()) {
		if (strcmp(c->Name(), "Sprite") == 0) {
			SpriteData data;


			data.spriteSheet = this;
			data.name = std::string(c->Attribute("Name"));

			c->QueryAttribute("X", &data.x);
			c->QueryAttribute("Y", &data.y);
			c->QueryAttribute("Width", &data.width);
			c->QueryAttribute("Height", &data.height);
			c->QueryAttribute("OriginX", &data.originX);
			c->QueryAttribute("OriginY", &data.originY);

			m_sprites.push_back(data);
		}
	}
}

const SpriteData* SpriteSheet::getSpriteData(std::string name) const {
	for (int i = 0; i < (int)m_sprites.size(); i++) {
		if (m_sprites[i].name == name) {
			return &m_sprites[i];
		}
	}

	return NULL;
}

sf::Sprite* SpriteSheet::makeSprite(std::string name) const {
	const SpriteData* spr = getSpriteData(name);

	if (spr != NULL) {
		return spr->makeSprite();
	}

	return NULL;
}

SpriteSheet::~SpriteSheet() {
	delete texture;
}

/*void BitmapFont::drawText(Rasterizer* rasterizer, std::string text, float x, float y) {
	float currentX = x;

	rasterizer->bindTexture(&texture);
	
	for (int c = 0; c < text.size(); c++) {
		BitmapChar bc = chars[text[c]];

		float charX = currentX + bc.xoffset;
		float charY = (y - lineHeight + base) + bc.yoffset;

		if (kerning[c].size() != 0 && c != 0) {
			for (int k = 0; k < kerning[c].size(); k++) {
				if (kerning[c][k].first == text[c - 1]) {
					charX += kerning[c][k].amount;
				}
			}
		}

		Vertex vertices[] = { { charX, charY, 0, bc.uvX, bc.uvY },
							 { charX + bc.width, charY, 0, bc.uvX + bc.uvWidth, bc.uvY },
							 { charX + bc.width, charY + bc.height, 0, bc.uvX + bc.uvWidth, bc.uvY + bc.uvHeight },
							 { charX, charY + bc.height, 0, bc.uvX, bc.uvY + bc.uvHeight } };

		currentX += bc.xadvance;

		rasterizer->drawQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
	}
}*/

ResourceFactory* ResourceFactory::s_instance = nullptr;

ResourceFactory::ResourceEntry* ResourceFactory::find(std::string _key) {
	for (ResourceEntry* m_resource : m_resources) {
		if (m_resource->m_key == _key)
			return m_resource;
	}

	return nullptr;
}

bool ResourceFactory::isKeyValid(std::string _key) {
	return _key != "" && find(_key) == nullptr;
}
 
bool ResourceFactory::keyCheck(std::string _key) {
	if (isKeyValid(_key)) {
		return true;
	}
	else {
		IO::Error("Key '" + _key + "' is either invalid or already taken.");
		return false;
	}
}

bool ResourceFactory::keyExists(std::string _key) {
	if (find(_key) == nullptr) {
		IO::Error("Key '" + _key + "' doesn't exist!");
		return false;
	}

	return true;
}

//void ResourceManager::loadDictionary(std::string _dictionaryPath) {}

void ResourceFactory::loadTexture(std::string _key, std::string path) {
	if (keyCheck(_key)) {
		sf::Texture* texture = new sf::Texture();

		if (!texture->loadFromFile(path)) {
			IO::Error("Unable to load texture with the path of '" + path + "'!");
			delete texture;
			return;
		}

		m_resources.push_back(new ResourceEntry(_key, ResourceEntry::EntryType::Texture, texture));
	}
}

void ResourceFactory::loadBitmapFont(std::string _key, std::string _fontXML, std::string _pagePath) {
	if (keyCheck(_key)) {
		BitmapFont* m_font = new BitmapFont(_fontXML.c_str(), { _pagePath });
		m_resources.push_back(new ResourceEntry(_key, ResourceEntry::EntryType::BitmapFont, m_font));
	}
}

void ResourceFactory::loadSprite(std::string _key, std::string _textureKey, int _x, int _y, int _width, int _height, float _originX, float _originY) {
	loadSprite(_key, _textureKey, { _x, _y, _width, _height }, { _originX, _originY });
}
void ResourceFactory::loadSprite(std::string _key, std::string _textureKey, sf::IntRect _region, sf::Vector2f _origin){
	if (keyCheck(_key)) {
		sf::Texture* tex = getTexture(_textureKey);

		if (tex != nullptr) {
			SpriteData* sprite = new SpriteData();
			sprite->rect = _region;
			sprite->origin = _origin;
			m_resources.push_back(new ResourceEntry(_key, ResourceEntry::EntryType::Sprite, sprite));
		}
	}
}

void ResourceFactory::loadMusic(std::string _key, std::string _musicPath) {
	if (keyCheck(_key)) {
		sf::Music* music = new sf::Music();

		if (!music->openFromFile(_musicPath)) {
			IO::Error("Unable to load texture with the path of '" + _musicPath + "'!");
			delete music;
			return;
		}

		m_resources.push_back(new ResourceEntry(_key, ResourceEntry::EntryType::Music, music));
	}
}

void ResourceFactory::loadSound(std::string _key, std::string _sfxPath) {
	if (keyCheck(_key)) {
		sf::SoundBuffer* buffer = new sf::SoundBuffer();

		if (!buffer->loadFromFile(_sfxPath)) {
			IO::Error("Unable to load texture with the path of '" + _sfxPath + "'!");
			delete buffer;
			return;
		}

		m_resources.push_back(new ResourceEntry(_key, ResourceEntry::EntryType::Sound, buffer));
	}
}

sf::Texture* ResourceFactory::getTexture(std::string _key) {
	return check<sf::Texture>(_key, ResourceEntry::EntryType::Texture);
}

BitmapFont* ResourceFactory::getBitmapFont(std::string _key) {
	return check<BitmapFont>(_key, ResourceEntry::EntryType::BitmapFont);
}

SpriteData* ResourceFactory::getSprite(std::string _key) {
	return check<SpriteData>(_key, ResourceEntry::EntryType::Sprite);
}

sf::Music* ResourceFactory::getMusic(std::string _key) {
	return check<sf::Music>(_key, ResourceEntry::EntryType::Music);
}

sf::SoundBuffer* ResourceFactory::getSound(std::string _key) {
	return check<sf::SoundBuffer>(_key, ResourceEntry::EntryType::Sound);
}

sf::Sprite* ResourceFactory::makeSFMLSprite(std::string _key) {
	sf::Sprite* sprite = new sf::Sprite();

	if (keyExists(_key)) {
		if (sf::Texture* texture = check<sf::Texture>(_key, ResourceEntry::EntryType::Texture)) {
			sprite->setTexture(*texture, true);
		} else if (SpriteData* data = check<SpriteData>(_key, ResourceEntry::EntryType::Sprite)) {
			sprite->setTexture(*data->m_texture);
			sprite->setTextureRect(data->rect);
		} else {
			delete sprite;
			IO::Error("Key '" + _key + "' is not a Texture or Sprite entry.");
			return nullptr;
		}

	}

	return sprite;
}

void ResourceFactory::clear() {
	for (ResourceEntry* resource : m_resources) {
		delete resource->m_ptr;
		delete resource;
	}
}

ResourceFactory& ResourceFactory::getInstance() {
	if (s_instance == nullptr)
		s_instance = new ResourceFactory();

	return *s_instance;
}