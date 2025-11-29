#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include "shooting.h"

// --- Configuration ---
const int FRAME_WIDTH = 50;
const int FRAME_HEIGHT = 60;
const float ENEMY_SPEED = 80.0f; 

class Enemy {
public:
    // Define the possible states
    enum State { Idle, Walking, Attacking, Hurt, Dying, Dead };
    
    // Struct to hold texture and frame count for a specific animation state
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
    
    // Setters & Getters
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    bool isAlive() const { return currentState != Dying && currentState != Dead; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

    // --- NEW: Bullet Access ---
    // Allows main.cpp to access the boss's bullets for collision checks
    std::vector<Bullet>& getBullets() { return bullets; }

private:
    sf::Sprite sprite;
    std::map<State, AnimData> animations; 
    State currentState;
    float currentFrameTime;
    int currentFrameIndex;
    float health;

    // --- NEW: Shooting Variables ---
    std::vector<Bullet> bullets;
    sf::Texture bulletTexture;
    float shootCooldown;

    // Helper functions
    void changeState(State newState);
    void updateAnimation(float deltaTime);
    void updateMovement(float deltaTime, const sf::Vector2f& targetPosition);
};