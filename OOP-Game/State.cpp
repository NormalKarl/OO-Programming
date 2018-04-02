#include "State.h"
#include "Game.h"

void Camera::update() {
	if(m_focused != NULL)
		setCenter(m_focused->getPosition());
}

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
	for (GameObject* gameObject : m_gameObjects) {
		gameObject->update();
	}

	for (GameObject* a : m_gameObjects) {
		for (GameObject* b : m_gameObjects) {
			if(a != b)
				a->collide(b);
		}
	}

	m_camera.update();
}

void State::draw(sf::RenderTarget& _target, sf::RenderStates _states) const {
	_target.setView(m_camera);

	for (GameObject* gameObject : m_gameObjects) {
		_target.draw(*gameObject, _states);
	}
}

void State::addGameObject(GameObject* object) {
	m_gameObjects.push_back(object);
}
