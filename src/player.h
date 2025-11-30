#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Shooting.h"
#include "gameObject.h"

class Player
{
    // --- PUBLIC FUNCTIONS ---
public:
    Player();
    void loadAssets();
    void handleEvents(sf::Event event);
    void update(float deltaTime, std::vector<Bullet> &bullets, sf::Texture &bulletTexture, std::vector<GameObject> &gameObjects); 
    void draw(sf::RenderWindow &window);
    sf::Vector2f getPosition() { return sprite.getPosition(); }
    sf::FloatRect getBounds() const;

    // --- Health System ---
    void takeDamage(float amount);
    float getHealth() const { return health; }

    // --- PRIVATE DATA ---
private:
    // --- Primary Sprite and Physics ---
    sf::Sprite sprite;
    sf::Vector2f velocity;

    // --- Health Variable ---
    float health;
    // FIX: Changed to static constexpr
    static constexpr float MAX_HEALTH = 100.f; 

    // --- TEXTURES ---
    sf::Texture idleTexture;
    sf::Texture idleShootTexture;
    sf::Texture runTexture;
    sf::Texture runShootTexture;
    sf::Texture jumpTexture;
    sf::Texture jumpShootTexture;
    sf::Texture crouchTexture;
    sf::Texture crouchShootTexture;
    sf::Texture dashTexture;
    sf::Texture doubleJumpTexture;
    sf::Texture shootTexture;

    // --- State Variables ---
    int jumpCount;
    int facingDirection; 
    bool onGround;
    bool isShooting;      
    bool isCrouching;     
    bool isDoubleJumping; 

    // --- Dash Timers & Cooldowns ---
    bool isDashing;
    bool canDash;
    float dashTimeLeft;
    float dashCooldownTime;

    // --- Animation Timers/Frames ---
    int animFrame;            
    float animTimer;          
    float textureSwitchTimer; 
    float jumpAnimTimer;
    
    // FIX: Changed to static constexpr
    static constexpr float JUMP_ANIM_SPEED = 0.2f;

    int currentFrameCount; 
    float shootTimer;      

    // --- CONSTANTS (FIXED) ---
    // Changed all "const" to "static constexpr" to allow player reset
    static constexpr float GRAVITY = 1000.f;
    static constexpr float MOVE_SPEED = 400.f;
    static constexpr float JUMP_SPEED = 500.f;
    static constexpr float DASH_SPEED = 1200.f;

    static constexpr int MAX_JUMPS = 2;
    static constexpr float ANIM_SPEED = 0.1f;
    static constexpr float DASH_DURATION = 0.2f;
    static constexpr float DASH_COOLDOWN = 1.0f;

    static constexpr int FRAME_HEIGHT = 48;
    static constexpr int FRAME_WIDTH = 48;

    static constexpr int IDLE_FRAMES = 4;
    static constexpr int RUN_FRAMES = 4;
    static constexpr int JUMP_FRAMES = 4;
    static constexpr int CROUCH_FRAMES = 4; 
    static constexpr int DASH_FRAMES = 4;
    static constexpr int DOUBLE_JUMP_FRAMES = 4;
    static constexpr int CROUCH_HOLD_FRAME = 2;
    static constexpr int SHOOT_FRAMES = 4;
};