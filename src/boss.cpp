#include "boss.h"
#include <cmath>
#include <iostream>

Enemy::Enemy()
    : currentState(Idle), 
      currentFrameTime(0.0f), 
      currentFrameIndex(0), 
      health(100.0f),
      shootCooldown(0.0f) // Initialize the firing timer
{
    // Initialize frame counts and speeds
    animations[Idle]      = {sf::Texture(), 4, 0.18f}; 
    animations[Walking]   = {sf::Texture(), 6, 0.10f}; 
    animations[Attacking] = {sf::Texture(), 8, 0.08f}; 
    animations[Hurt]      = {sf::Texture(), 2, 0.15f};
    animations[Dying]     = {sf::Texture(), 5, 0.25f};

    // Set Initial Scale (3x to match Player)
    sprite.setScale(3.0f, 3.0f);
}

bool Enemy::loadResources() {
    // Load all animation textures
    if (!animations[Idle].texture.loadFromFile("../assets/Idle.png")) return false;
    if (!animations[Walking].texture.loadFromFile("../assets/Walk.png")) return false;
    if (!animations[Attacking].texture.loadFromFile("../assets/Attack.png")) return false;
    if (!animations[Hurt].texture.loadFromFile("../assets/Hurt.png")) return false;
    if (!animations[Dying].texture.loadFromFile("../assets/Death.png")) return false;

    // --- NEW: Load Bullet Texture ---
    // The Boss needs this texture to create bullets
    if (!bulletTexture.loadFromFile("../assets/Bullet.png")) return false;

    // Set initial state
    changeState(Idle);
    sprite.setOrigin(FRAME_WIDTH / 2.0f, FRAME_HEIGHT / 2.0f);
    
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
    // If dead, stay dead
    if (currentState == Dying && currentFrameIndex == animations[Dying].frameCount - 1) {
        currentState = Dead; 
        return;
    }
    if (currentState == Dead) return;

    currentFrameTime += deltaTime;
    const AnimData& currentAnim = animations[currentState];

    // Frame switching logic
    if (currentFrameTime >= currentAnim.frameDuration) {
        currentFrameTime -= currentAnim.frameDuration;
        currentFrameIndex++;

        // Loop or Transition
        if (currentFrameIndex >= currentAnim.frameCount) {
            if (currentState == Dying) {
                currentFrameIndex = currentAnim.frameCount - 1; // Freeze on death
            } else if (currentState == Hurt || currentState == Attacking) {
                changeState(Walking); // Go back to walking after attack/hurt
            } else {
                currentFrameIndex = 0; // Loop idle/walk
            }
        }
    }

    sprite.setTextureRect(sf::IntRect(currentFrameIndex * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
}

void Enemy::updateMovement(float deltaTime, const sf::Vector2f& targetPosition) {
    // Don't move if busy
    if (currentState == Dying || currentState == Dead || currentState == Hurt || currentState == Attacking) {
        return;
    }

    sf::Vector2f directionVec = targetPosition - sprite.getPosition();
    float distance = std::hypot(directionVec.x, directionVec.y);
    
    const float ATTACK_RANGE = 50.0f; 
    // Shooting ranges: Closer than 700px, but further than 100px (so he melee attacks when close)
    const float SHOOT_RANGE_MIN = 100.0f;
    const float SHOOT_RANGE_MAX = 700.0f;

    // --- SHOOTING LOGIC ---
    shootCooldown -= deltaTime;
    
    if (distance > SHOOT_RANGE_MIN && distance < SHOOT_RANGE_MAX && shootCooldown <= 0.0f) {
        // 1. Determine direction
        float dir = (targetPosition.x > sprite.getPosition().x) ? 1.0f : -1.0f;
        
        // 2. Determine spawn point (Chest height)
        sf::Vector2f spawnPos = sprite.getPosition();
        spawnPos.y -= 20.0f; 
        
        // 3. Create Bullet
        bullets.push_back(Bullet(bulletTexture, spawnPos, dir));
        
        // 4. Reset Cooldown (Fire every 2 seconds)
        shootCooldown = 2.0f; 
        
        // 5. Face the player when shooting
        if (dir > 0) sprite.setScale(3.0f, 3.0f);
        else sprite.setScale(-3.0f, 3.0f);
    }

    // --- MOVEMENT LOGIC ---
    if (distance > ATTACK_RANGE) {
        changeState(Walking);

        // Simple movement toward player
        if (distance > 1.0f) {
            directionVec /= distance; // Normalize
            sprite.move(directionVec * ENEMY_SPEED * deltaTime);

            // Face direction of movement
            if (directionVec.x > 0) {
                sprite.setScale(3.0f, 3.0f);
            } else if (directionVec.x < 0) {
                sprite.setScale(-3.0f, 3.0f);
            }
        }
    } else {
        // Close enough to punch
        changeState(Attacking); 
    }
}

void Enemy::update(float deltaTime, const sf::Vector2f& targetPosition) {
    if (!isAlive()) return; 

    updateMovement(deltaTime, targetPosition);
    updateAnimation(deltaTime);

    // --- UPDATE BOSS BULLETS ---
    for (size_t i = 0; i < bullets.size(); ) {
        bullets[i].update(deltaTime);
        
        // Cleanup: Remove bullets that fly too far away (2000 pixels)
        if (std::abs(bullets[i].bulletsprite.getPosition().x - sprite.getPosition().x) > 2000.f) {
             bullets.erase(bullets.begin() + i);
        } else {
             ++i;
        }
    }
}

void Enemy::takeDamage(int damage) {
    if (!isAlive()) return;

    health -= damage;
    if (health <= 0) {
        changeState(Dying);
    } else if (currentState != Hurt) {
        changeState(Hurt);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    if (currentState != Dead) {
        window.draw(sprite);
    }
    
    // Draw bullets (even if Boss is dead, existing bullets should persist briefly)
    for (auto& bullet : bullets) {
        bullet.draw(window);
    }
}