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

	getCamera().update();
}

void State::draw(sf::RenderTarget& _target, sf::RenderStates _states) const {
	_target.setView(getCamera());

	for (GameObject* gameObject : m_gameObjects) {
		_target.draw(*gameObject, _states);
	}
}

void State::addGameObject(GameObject* object) {
	object->m_parent = this;
	m_gameObjects.push_back(object);
}

void State::setCamera(Camera* camera) {
	if (m_cameras.empty())
		m_cameras.resize(1);

	m_cameras[0] = camera;
}