#include "State.h"
#include "Game.h"

State::State(std::string name) : m_name(name) {
	m_game = NULL;
}

State::~State()
{
	if (m_game != NULL) {
		m_game->removeState(this);
	}
}

void State::update() {

}


void State::draw(sf::RenderTarget& _target, sf::RenderStates _states) const {

}
