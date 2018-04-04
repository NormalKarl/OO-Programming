#include "Graphics.h"
#include "Game.h"

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

