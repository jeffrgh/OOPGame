#include "gameObject.h"

GameObject::GameObject(sf::Vector2f position, sf::Vector2f size)
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(sf::Color::Red);
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color::Blue);
}

void GameObject::draw(sf::RenderWindow &window) const
{
	window.draw(shape);
}

sf::FloatRect GameObject::getBounds() const
{
	return shape.getGlobalBounds();
}

sf::Vector2f GameObject::getPosition() const
{
	return shape.getPosition();
}
