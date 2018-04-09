#include "GameObject.h"
#include "Input.h"
#include "TileMap.h"
#include "Graphics.h"
#include "State.h"

GameObject::GameObject(bool _persistent) {
	m_state = NULL;
	m_parent = NULL;
	m_relativeToView = true;
	m_depth = 0;
	m_persistent = _persistent;
}

GameObject::~GameObject() {
	for (size_t i = 0; i < m_children.size(); i++) {
		if (!m_children[i]->isPersistent())
			delete m_children[i];
	}
}

void GameObject::update() {
	for (const sf::Drawable* drawable : m_graphics) {
		if (dynamic_cast<const Graphic*>(drawable) != NULL) {
			((Graphic*)drawable)->update();
		}
	}

	for (GameObject* child : m_children) {
		child->update();
	}
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	const sf::View& v = target.getView();

	if (!m_relativeToView) {
		sf::View v2 = v;
		v2.setCenter(v2.getSize() / 2.0f);
		target.setView(v2);
	}

	states.transform *= getTransform();

	for (const sf::Drawable* drawable : m_graphics) {
		target.draw(*drawable, states);
	}

	for (GameObject* child : m_children) {
		target.draw(*child, states);
	}

	if (!m_relativeToView) {
		target.setView(v);
	}
}

const sf::Drawable* GameObject::getGraphic() {
	return m_graphics.size() != 0 ? m_graphics[0] : NULL;
}

void GameObject::setGraphic(const sf::Drawable* _graphic) {
	if (m_graphics.empty())
		m_graphics.resize(1);

	m_graphics[0] = _graphic;
}

std::vector<const sf::Drawable*>* GameObject::getGraphics() {
	return &m_graphics;
}

void GameObject::addGraphic(const sf::Drawable* _graphic) {
	m_graphics.push_back(_graphic);
}


void GameObject::addChild(GameObject* _child) {
	m_children.push_back(_child);
	_child->m_parent = this;
}

void GameObject::setBoundingBox(float width, float height, float originX, float originY) {
	aabb = { width, height };
	aabbOrigin = { originX, originY };
}

sf::FloatRect GameObject::getBoundingBox() {
	return sf::FloatRect(getPosition() - aabbOrigin, aabb);
}

bool GameObject::intersect(sf::Vector2i point) {
	return intersect((sf::Vector2f)point);
}

bool GameObject::intersect(sf::Vector2f point) {
	sf::FloatRect boundingBox = getBoundingBox();

	return (point.x >= boundingBox.left && point.x < boundingBox.left + boundingBox.width)
		&& (point.y >= boundingBox.top && point.y < boundingBox.top + boundingBox.height);
}

bool GameObject::intersect(GameObject* other) {
	return getBoundingBox().intersects(other->getBoundingBox());
}

void GameObject::setDepth(int depth) {
	this->m_depth = depth;

	if (m_state != NULL) {
		m_state->reorder();
	}
}