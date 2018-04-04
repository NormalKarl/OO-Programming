#include "Graphics.h"
#include "Game.h"
#include "BitmapFont.h"

Animation::Animation(const sf::Texture* texture, std::vector<sf::IntRect> frames, float speed) : m_speed(speed) {
	for (sf::IntRect rect : frames) {
		m_frames.push_back(sf::Sprite(*texture, rect));
	}
}

void Animation::update() {
	if (m_playing && m_speed != 0) {
		m_elapsed += Game::getGame()->getDelta();

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
}

Label::~Label() {
}

void Label::setText(std::string text)  {
	float currentX = 0;

	for (int c = 0; c < text.size(); c++) {
		const BitmapChar bc = font->getChars()[text[c]];

		float charX = currentX + bc.xoffset;
		float charY = (font->getLineHeight() + font->getBase()) + bc.yoffset;

		if (font->getKernings()[c].size() != 0 && c != 0) {
			for (int k = 0; k < font->getKernings()[c].size(); k++) {
				if (font->getKernings()[c][k].first == text[c - 1]) {
					charX += font->getKernings()[c][k].amount;
				}
			}
		}

		currentX += bc.xadvance;

		va.append({ { charX, charY }, { bc.uvX, bc.uvY } });
		va.append({ { charX + bc.width, charY }, { bc.uvX + bc.uvWidth, bc.uvY } });
		va.append({ { charX + bc.width, charY + bc.height }, { bc.uvX + bc.uvWidth, bc.uvY + bc.uvHeight } });
		va.append({ { charX, charY + bc.height }, { bc.uvX, bc.uvY + bc.uvHeight } });
	}
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.texture = font->getTexture();
	states.transform *= getTransform();
	target.draw(va, states);
}
