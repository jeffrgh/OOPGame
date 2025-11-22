#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>
#include "Shooting.h" // Assuming this defines the Bullet structure

class Player
{
// --- PUBLIC FUNCTIONS (External Control) ---
public:
    Player(); 
    void loadAssets();
    // deltaTime is the time since the last frame
    void update(float deltaTime, std::vector<Bullet>& bulletList, sf::Texture& bulletTexture); 
    void handleEvents(sf::Event event); 
    void draw(sf::RenderWindow& window); 
    
    // Required for the Camera system
    sf::Vector2f getPosition() const { return sprite.getPosition(); } 

// --- PRIVATE DATA (The Player's State and Assets) ---
private:
    // --- Primary Sprite and Physics ---
    sf::Sprite sprite;
    sf::Vector2f velocity;
    
    // --- CHARACTER ANIMATION TEXTURES (10 Required Unique States) ---
    sf::Texture idleTexture;        // Idle (No Shoot)
    sf::Texture idleShootTexture;   // Idle + Shoot
    
    sf::Texture runTexture;         // Run (No Shoot)
    sf::Texture runShootTexture;    // Run + Shoot
    
    sf::Texture jumpTexture;        // Jump (No Shoot)
    sf::Texture jumpShootTexture;   // Jump + Shoot
    
    sf::Texture crouchTexture;      // Crouch (No Shoot)
    sf::Texture crouchShootTexture; // Crouch + Shoot
    
    sf::Texture dashTexture;        // Dash (No Shoot variation needed)
    sf::Texture doubleJumpTexture;  // Double Jump (No Shoot variation needed)
    
    // --- State Variables ---
    int jumpCount; 
    int facingDirection; // 1 for right, -1 for left
    
    bool onGround;
    bool isShooting;        // True if the shoot key is pressed
    bool isCrouching;       // True if the crouch key is held
    bool isDoubleJumping;   // True when character is performing the second jump
    
    // --- Dash Timers & Cooldowns ---
    bool isDashing; 
    bool canDash; 
    float dashTimeLeft; 
    float dashCooldownTime; 
    
    // --- Animation Timers/Frames ---
    int animFrame;              // Current index in the sprite sheet row
    float animTimer;            // Timer for frame speed
    float textureSwitchTimer;   // Timer to control how often the state machine runs
    float jumpAnimTimer;
const float JUMP_ANIM_SPEED = 0.2f;

    int currentFrameCount;      // Total frames for the current animation
    float shootTimer;           // Timer to control fire rate (optional)

    // --- CONSTANTS ---
    // Physics
    const float GRAVITY = 1000.f;
    const float MOVE_SPEED = 400.f;
    const float JUMP_SPEED = 500.f;
    const float DASH_SPEED = 1200.f;
    
    // Limits and Durations
    const int MAX_JUMPS = 2;
    const float ANIM_SPEED = 0.1f;
    const float DASH_DURATION = 0.2f; 
    const float DASH_COOLDOWN = 1.0f;
    
    // Sprite Dimensions
    const int FRAME_HEIGHT = 48;
    const int FRAME_WIDTH = 48;
    
    // Frame Counts (Crucial: Crouch is set to 4 frames as requested)
    const int IDLE_FRAMES = 4;
    const int RUN_FRAMES = 4;
    const int JUMP_FRAMES = 4;
    const int CROUCH_FRAMES = 4;    // <--- Set to 4 frames for the hold logic
    const int DASH_FRAMES = 4;
    const int DOUBLE_JUMP_FRAMES = 4;
    const int CROUCH_HOLD_FRAME = 2;
};



