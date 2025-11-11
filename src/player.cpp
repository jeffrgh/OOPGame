#include "Player.h" // Include the blueprint we just made
#include <iostream> // For error messages

Player::Player()
{
    velocity = sf::Vector2f(0.f, 0.f);
    onGround = true;
    animFrame = 0;
    animTimer = 0.f;
    currentFrameCount = IDLE_FRAMES;
}

// This function loads all the textures the player needs
void Player::loadAssets()
{
    if (!walkTexture.loadFromFile("../assets/Run.png"))
    {
        std::cerr << "Error loading Run.png" << std::endl;
    }
    if (!idleTexture.loadFromFile("../assets/Idle.png"))
    {
        std::cerr << "Error loading Idle.png" << std::endl;
    }
    if (!jumpTexture.loadFromFile("../assets/Jump.png"))
    {
        std::cerr << "Error loading Jump.png" << std::endl;
        // Fallback: If Jump.png is missing, just use Idle.png
        jumpTexture = idleTexture;
    }
    if(!shootTexture.loadFromFile("../assets/Shot.png"))
    {
            std::cerr << "Error loading Shot.png " << std::endl;
            shootTexture = idleTexture;
    }

    // Set the initial texture and setup the sprite
    sprite.setTexture(idleTexture);
    sprite.setPosition(100.f, 950.f);
    sprite.setOrigin(FRAME_WIDTH / 2.f, FRAME_HEIGHT);
    sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
    sprite.setScale(3.f, 3.f);
}

// This handles "one-shot" inputs, like jumping
void Player::handleEvents(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if ((event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Up) && onGround)
        {
            velocity.y = -JUMP_SPEED;
            onGround = false;
        }
    }
}

void Player::update(float deltaTime)
{
    // --- Real-Time Input (Walking) ---
    velocity.x = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        velocity.x = -MOVE_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        velocity.x = MOVE_SPEED;
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        isShooting = true;
    }
    else{
        isShooting = false;
    }

    // --- Sprite Flipping ---
    if (velocity.x > 0.f)
        sprite.setScale(3.f, 3.f);
    else if (velocity.x < 0.f)
        sprite.setScale(-3.f, 3.f);

    // --- State Machine (Animation) ---
    const sf::Texture *oldTexture = sprite.getTexture();


    if (isShooting)
    {
        sprite.setTexture(shootTexture);
        currentFrameCount = SHOOT_FRAMES;
    }
    else if (!onGround & velocity.x == 0.f)
    {
        sprite.setTexture(jumpTexture);
        currentFrameCount = JUMP_FRAMES;
    }
    else if (!onGround & velocity.x != 0.f)
    {
        sprite.setTexture(walkTexture);
        currentFrameCount = JUMP_FRAMES;
    }
    else if (velocity.x != 0.f)
    {
        sprite.setTexture(walkTexture);
        currentFrameCount = RUN_FRAMES;
    }
    else
    {
        sprite.setTexture(idleTexture);
        currentFrameCount = IDLE_FRAMES;
    }
    // If we changed textures, reset the animation
    if (oldTexture != sprite.getTexture())
    {
        animFrame = 0;
        animTimer = 0.f; // Also reset timer
    }

    // --- Physics (Gravity and Ground Check) ---
    if (!onGround)
    {
        velocity.y += GRAVITY * deltaTime;
    }

    // --- Final Movement (Must be BEFORE ground check) ---
    sprite.move(velocity * deltaTime);

    // --- Ground Collision Check (Must be AFTER moving) ---
    if (sprite.getPosition().y >= 950.f)
    {
        if (velocity.y > 0) // Only stop if we were falling
        {
            sprite.setPosition(sprite.getPosition().x, 950.f);
            onGround = true;
            velocity.y = 0;
        }
    }

    // --- Animation Timer ---
    animTimer += deltaTime;
    if (animTimer >= ANIM_SPEED)
    {
        animTimer = 0.f;
        animFrame++;

        if (animFrame >= currentFrameCount)
        {
            animFrame = 0;
        }
    }

    // --- Update the "cookie cutter" ---
    sprite.setTextureRect(sf::IntRect(
        animFrame * FRAME_WIDTH,
        0, // Row is 0 (as each texture is its own file)
        FRAME_WIDTH,
        FRAME_HEIGHT));

    
}

// This just draws the sprite to the window
void Player::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}