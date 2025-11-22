#include "shooting.h"

Bullet::Bullet(sf::Texture &texture, sf::Vector2f startPos, float direction)
{
    bulletsprite.setTexture(texture);
    bulletsprite.setPosition(startPos);

    // Set the bullet's velocity based on the direction
    velocity.x = BULLET_SPEED * direction;
    velocity.y = 0.f; // No vertical movement
}

void Bullet::update(float deltaTime)
{
    // Move the bullet every frame
    bulletsprite.move(velocity * deltaTime);
}

void Bullet::draw(sf::RenderWindow &window)
{
    window.draw(bulletsprite);
}