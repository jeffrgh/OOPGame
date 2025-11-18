#pragma once // Stops this file from being included multiple times
#include <SFML/Graphics.hpp>
#include "Shooting.h"
#include "gameObject.h"
class Player
{
    // --- PUBLIC FUNCTIONS (What other files can tell the Player to do) ---
public:
    Player();                                                                                                                        // The "Constructor": called when a new Player is created
    void loadAssets();                                                                                                               // A function to load all our textures
    void handleEvents(sf::Event event);                                                                                              // Handles one-shot inputs like jump
    void update(float deltaTime, std::vector<Bullet> &bulletList, sf::Texture &bulletTexture, std::vector<GameObject> &gameObjects); // Updates all physics, input, and animation
    void draw(sf::RenderWindow &window);                                                                                             // Draws the player to Rendethe w
    sf::Vector2f getPosition();
    sf::FloatRect getBounds() const;

    // --- PRIVATE DATA (Stuff the Player manages for itself) ---
private:
    sf::Sprite sprite;
    sf::Texture idleTexture;
    sf::Texture walkTexture;
    sf::Texture jumpTexture;
    sf::Texture shootTexture;

    sf::Vector2f velocity;
    bool onGround;
    bool isShooting;

    int animFrame;
    float animTimer;
    float textureSwitchTimer;
    int currentFrameCount;

    // Constants can also be private
    const float GRAVITY = 1000.f;
    const float MOVE_SPEED = 400.f;
    const float JUMP_SPEED = 500.f;
    const float ANIM_SPEED = 0.1f;
    const int FRAME_WIDTH = 128;
    const int FRAME_HEIGHT = 128;
    const int IDLE_FRAMES = 6;
    const int RUN_FRAMES = 6;
    const int JUMP_FRAMES = 2;
    const int SHOOT_FRAMES = 4;
};