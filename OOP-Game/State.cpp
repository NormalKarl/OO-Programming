#include "State.h"
#include "Game.h"

State::State(std::string name) : m_name(name) {
	sf::Vector2f size =  (sf::Vector2f) Game::getInstance().getSize();
	setCamera(sf::View({0, 0, size.x, size.y}));
}

State::~State()
{
	for (size_t i = 0; i < m_gameObjects.size(); i++) {
		if (!m_gameObjects[i]->isPersistent())
			delete m_gameObjects[i];
	}
}

void State::update() {
	for (GameObject* gameObject : m_gameObjects) {
		gameObject->update();
	}

	for (GameObject* gameObject : m_gameObjects) {
		if (gameObject->m_destroy) {
			m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), gameObject));
		}
	}

	if (m_pollOrder) {
		pollOrder();
		m_pollOrder = false;
	}
}

void State::draw(sf::RenderTarget& _target, sf::RenderStates _states) const {
	_target.clear(m_backgroundColour);
	_target.setView((sf::View&)m_camera);

	for (GameObject* gameObject : m_gameObjects) {
		_target.draw(*gameObject, _states);
	}
}

sf::Vector2f State::map(sf::Vector2i _pixel) {
	sf::RenderWindow* window = Game::getInstance().getWindow();

	sf::View view = window->getView();
	window->setView(m_camera);

	sf::Vector2f position = window->mapPixelToCoords(_pixel);
	window->setView(view);

	return position;
}

void State::add(GameObject* object) {
	object->m_state = this;
	m_gameObjects.push_back(object);
}

void State::pollOrder() {
	std::sort(m_gameObjects.begin(), m_gameObjects.end(),
		[](const GameObject* a, const GameObject* b) {
			return a->getDepth() < b->getDepth();
		}
	);
}

void State::reorder() {
	m_pollOrder = true;
}