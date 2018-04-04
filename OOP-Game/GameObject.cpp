#include "GameObject.h"
#include "Input.h"
#include "TileMap.h"
#include "Graphics.h"
#include "Collider.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::update() {
	for (const sf::Drawable* drawable : m_graphics) {
		if (dynamic_cast<const Updatable*>(drawable) != NULL) {
			((Updatable*)drawable)->update();
		}
	}
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	for (const sf::Drawable* drawable : m_graphics) {
		target.draw(*drawable, states);
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

Collider* GameObject::getCollider() {
	return m_colliders.size() != 0 ? m_colliders[0] : NULL;
}

void GameObject::setCollider(Collider* collider) {
	if (m_colliders.empty())
		m_colliders.resize(1);

	collider->m_parent = this;
	m_colliders[0] = collider;
}

std::vector<Collider*>* GameObject::getColliders() {
	return &m_colliders;
}

void GameObject::addCollider(Collider* collider) {
	collider->m_parent = this;
	m_colliders.push_back(collider);
}