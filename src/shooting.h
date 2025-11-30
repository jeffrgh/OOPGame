#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
public:
    sf::Sprite bulletsprite; // The bullet's sprite

    // Constructor: Runs when a new bullet is created
    Bullet(sf::Texture& texture, sf::Vector2f startPos, float direction);

    void update(float deltaTime); // Moves the bullet
    void draw(sf::RenderWindow& window); // Draws the bullet

private:
    sf::Vector2f velocity;
    static constexpr float BULLET_SPEED = 800.f;
};