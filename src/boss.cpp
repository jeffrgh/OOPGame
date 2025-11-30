#include "boss.h"
#include <cmath>
#include <iostream>

Enemy::Enemy()
    : currentState(Idle), 
      currentFrameTime(0.0f), 
      currentFrameIndex(0), 
      health(500.0f),
      shootCooldown(0.0f) 
{

    animations[Idle]      = {sf::Texture(), 4, 0.18f}; 
    animations[Walking]   = {sf::Texture(), 6, 0.10f}; 
    animations[Attacking] = {sf::Texture(), 8, 0.08f}; 
    animations[Hurt]      = {sf::Texture(), 2, 0.15f};
    animations[Dying]     = {sf::Texture(), 5, 0.25f};

    sprite.setScale(3.0f, 3.0f);
}

bool Enemy::loadResources() {
    if (!animations[Idle].texture.loadFromFile("../assets/Idle.png")) return false;
    if (!animations[Walking].texture.loadFromFile("../assets/Walk.png")) return false;
    if (!animations[Attacking].texture.loadFromFile("../assets/Attack.png")) return false;
    if (!animations[Hurt].texture.loadFromFile("../assets/Hurt.png")) return false;
    if (!animations[Dying].texture.loadFromFile("../assets/Death.png")) return false;
    

    if (!bulletTexture.loadFromFile("../assets/Bullet.jpg")) {
        std::cerr << "BOSS ERROR: Could not load Bullet.png" << std::endl;
        return false;
    }

    changeState(Idle);
    sprite.setOrigin(FRAME_WIDTH / 2.0f, FRAME_HEIGHT);
    return true;
}

void Enemy::changeState(State newState) {
    if (currentState == newState) return; 
    currentState = newState;
    currentFrameIndex = 0;
    currentFrameTime = 0.0f;

    const AnimData& data = animations[newState];
    sprite.setTexture(data.texture);
    sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
}

void Enemy::updateAnimation(float deltaTime) {

    if (currentState == Dead) return;

    currentFrameTime += deltaTime;
    const AnimData& currentAnim = animations[currentState];

    if (currentFrameTime >= currentAnim.frameDuration) {
        currentFrameTime -= currentAnim.frameDuration;
        currentFrameIndex++;

        // End of animation loop
        if (currentFrameIndex >= currentAnim.frameCount) {
            if (currentState == Dying) {
                currentFrameIndex = currentAnim.frameCount - 1; 
                currentState = Dead; 
            } else if (currentState == Hurt || currentState == Attacking) {
     
                changeState(Walking); 
            } else {
    
                currentFrameIndex = 0; 
            }
        }
    }
    sprite.setTextureRect(sf::IntRect(currentFrameIndex * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
}

void Enemy::updateMovement(float deltaTime, const sf::Vector2f& targetPosition) {

    sprite.setPosition(sprite.getPosition().x, 950.f);


    float diffX = targetPosition.x - sprite.getPosition().x;
    float absDistX = std::abs(diffX);

    if (currentState != Dying && currentState != Dead && currentState != Hurt) 
    {
        shootCooldown -= deltaTime;
        const float SHOOT_RANGE = 2000.0f; 


        if (absDistX < SHOOT_RANGE && shootCooldown <= 0.0f) {
            float dir = (diffX > 0) ? 1.0f : -1.0f;
            sf::Vector2f spawnPos = sprite.getPosition();
            spawnPos.y -= 60.0f; 
            
            bullets.push_back(Bullet(bulletTexture, spawnPos, dir));
            shootCooldown = 1.0f; 
            
            std::cout << "BOSS FIRED BULLET!" << std::endl; 
        }
    }

    if (currentState == Dying || currentState == Dead || currentState == Hurt || currentState == Attacking) {
        return;
    }

    const float MELEE_RANGE = 50.0f; 

    if (absDistX > MELEE_RANGE) {
        changeState(Walking);
        
        float moveDir = (diffX > 0) ? 1.0f : -1.0f;
        sprite.move(moveDir * ENEMY_SPEED * deltaTime, 0.f);

        if (moveDir > 0) sprite.setScale(3.0f, 3.0f);
        else sprite.setScale(-3.0f, 3.0f);
    } else {

        changeState(Attacking); 
    }
}

void Enemy::update(float deltaTime, const sf::Vector2f& targetPosition) {

    for (size_t i = 0; i < bullets.size(); ) {
        bullets[i].update(deltaTime);

        if (std::abs(bullets[i].bulletsprite.getPosition().x - sprite.getPosition().x) > 2500.f) {
             bullets.erase(bullets.begin() + i);
        } else {
             ++i;
        }
    }

    if (currentState == Dead) return; 
    updateMovement(deltaTime, targetPosition);
    updateAnimation(deltaTime);
}

void Enemy::takeDamage(int damage) {
    if (currentState == Dead || currentState == Dying) return;
    
    health -= damage;
    std::cout << "BOSS HEALTH: " << health << std::endl;

    if (health <= 0) {
        changeState(Dying);
    } else if (currentState != Hurt) {
        changeState(Hurt);
    }
}

void Enemy::draw(sf::RenderWindow& window) {

    window.draw(sprite);
    for (auto& bullet : bullets) {
        bullet.draw(window);
    }
}