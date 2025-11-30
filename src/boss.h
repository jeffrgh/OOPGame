#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include "shooting.h"

const int FRAME_WIDTH = 48; 
const int FRAME_HEIGHT = 48;
const float ENEMY_SPEED = 80.0f; 

class Enemy {
public:
    enum State { Idle, Walking, Attacking, Hurt, Dying, Dead };
    
    struct AnimData {
        sf::Texture texture;
        int frameCount;
        float frameDuration; 
    };

    Enemy();

    bool loadResources();
    void update(float deltaTime, const sf::Vector2f& targetPosition);
    void draw(sf::RenderWindow& window);
    void takeDamage(int damage);
    
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    bool isAlive() const { return currentState != Dead; } // Changed logic slightly
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    std::vector<Bullet>& getBullets() { return bullets; }

    // --- NEW: Helper for Melee Damage ---
    bool isMeleeAttacking() const { return currentState == Attacking; }

private:
    sf::Sprite sprite;
    std::map<State, AnimData> animations; 
    State currentState;
    float currentFrameTime;
    int currentFrameIndex;
    float health;

    std::vector<Bullet> bullets;
    sf::Texture bulletTexture;
    float shootCooldown;

    void changeState(State newState);
    void updateAnimation(float deltaTime);
    void updateMovement(float deltaTime, const sf::Vector2f& targetPosition);
};