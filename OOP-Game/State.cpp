#include "State.h"
#include "Game.h"

void Camera::update() {
	if(m_focused != NULL)
		setCenter(m_focused->getPosition());

	return;
}

sf::Vector2f Camera::mapPixelToCoords(sf::Vector2i mousePos) {
	m_parent->getGame()->getWindow()->setView(*this);
	return m_parent->getGame()->getWindow()->mapPixelToCoords(mousePos);
}

sf::Vector2f Camera::mapDistance(sf::Vector2i pixelDistance) {
	sf::Vector2f ndc;
	ndc.x = (float)getViewport().width / (float)m_parent->getGame()->getWindow()->getSize().x;
	ndc.y = (float)getViewport().height / (float)m_parent->getGame()->getWindow()->getSize().y;

	return sf::Vector2f((float)pixelDistance.x * ndc.x, (float)pixelDistance.y * ndc.y);
}

State::State(std::string name) : m_name(name) {
	m_game = NULL;

	Camera* c = new Camera();
	sf::Vector2f size = (sf::Vector2f)(Game::getGame()->getSize());
	c->setCenter(sf::Vector2f((float)size.x / 2.0f, (float)size.y / 2.0f));
	c->setSize(size);
	m_cameras.push_back(c);
}

State::~State()
{
	if (m_game != NULL) {
		m_game->removeState(this);
	}

	for (size_t i = 0; i < m_gameObjects.size(); i++) {
		if (!m_gameObjects[i]->isPersistent())
			delete m_gameObjects[i];
	}
}

void State::update() {
	for (GameObject* gameObject : m_gameObjects) {
		gameObject->update();
	}

	for (Camera* camera : m_cameras) {
		camera->update();
	}

	if (m_pollOrder) {
		pollOrder();
		m_pollOrder = false;
	}
}

void State::draw(sf::RenderTarget& _target, sf::RenderStates _states) const {
	_target.clear(m_clearColor);
	_target.setView(getCamera());

	for (GameObject* gameObject : m_gameObjects) {
		_target.draw(*gameObject, _states);
	}
}

void State::addGameObject(GameObject* object) {
	object->m_state = this;
	m_gameObjects.push_back(object);
}

void State::setCamera(Camera* camera) {
	if (m_cameras.empty())
		m_cameras.resize(1);

	camera->m_parent = this;
	m_cameras[0] = camera;
}

void State::pollOrder() {
	std::sort(m_gameObjects.begin(), m_gameObjects.end(),
		[](const GameObject* a, const GameObject* b) {
			return a->getDepth() < b->getDepth();
		}
	);

	std::sort(m_cameras.begin(), m_cameras.end(),
		[](const Camera* a, const Camera* b) {
			return a->getDepth() < b->getDepth();
		}
	);
}

void State::reorder() {
	m_pollOrder = true;
}