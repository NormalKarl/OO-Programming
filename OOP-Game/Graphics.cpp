#include "Graphics.h"
#include "Game.h"
#include "Resources.h"

Animation::Animation(const sf::Texture* texture, std::vector<sf::IntRect> frames, float speed) : m_speed(speed) {
	for (sf::IntRect rect : frames) {
		m_frames.push_back(sf::Sprite(*texture, rect));
	}
}

void Animation::update() {
	if (m_playing && m_speed != 0) {
		m_elapsed += Game::getInstance().getDelta();

		if (m_elapsed >= m_speed * m_frames.size()) {
			m_elapsed -= m_speed * m_frames.size();
		}
	}
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	int frame = m_speed != 0 ? (int)(m_elapsed / m_speed) : 0;

	states.texture = texture;
	states.transform *= getTransform();
	target.draw(m_frames[frame], states);
}

Label::Label(const BitmapFont* font, std::string text) {
	this->font = font;
	va = sf::VertexArray(sf::PrimitiveType::Quads);
	setText(text);
}

Label::~Label() {
}

void Label::setText(std::string text)  {
	float currentX = 0;

	for (int c = 0; c < (int)text.size(); c++) {
		const BitmapChar bc = font->getChars()[text[c]];

		float charX = currentX + bc.xoffset;
		float charY = bc.yoffset;
		//(font->getLineHeight() + font->getBase())

		if (font->getKernings()[c].size() != 0 && c != 0) {
			for (int k = 0; k < (int)font->getKernings()[c].size(); k++) {
				if (font->getKernings()[c][k].first == text[c - 1]) {
					charX += font->getKernings()[c][k].amount;
				}
			}
		}

		currentX += bc.xadvance;

		va.append({ { charX, charY },{ (float)bc.x, (float)bc.y } });
		va.append({ { charX + bc.width, charY },{ (float)bc.x + bc.width, (float)bc.y } });
		va.append({ { charX + bc.width, charY + bc.height },{ (float)bc.x + bc.width, (float)bc.y + bc.height } });
		va.append({ { charX, charY + bc.height },{ (float)bc.x, (float)bc.y + bc.height } });
	}

	m_labelSize = { currentX, (float)font->getLineHeight() };
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = font->getTexture();
	states.transform *= getTransform();
	target.draw(va, states);
}
