#include <iostream>
#include "player.h"
#include <algorithm>
#include "shooting.h" // Assuming this defines the Bullet structure

Player::Player()
{
    velocity = sf::Vector2f(0.f, 0.f);
    jumpCount = 0;
    isDoubleJumping = false;
    onGround = true;
    animFrame = 0;
    animTimer = 0.f;
    textureSwitchTimer = 0.f;
    currentFrameCount = IDLE_FRAMES;
    isDashing = false;
    canDash = true;
    dashTimeLeft = 0.0f;
    dashCooldownTime = 0.0f;
    facingDirection = 1;
    isCrouching = false;
    isShooting = false;
    shootTimer = 0.0f; 
    jumpAnimTimer = 0.0f;
    health = MAX_HEALTH;
}


void Player::takeDamage(float amount)
{
    health -= amount;
    if (health < 0) health = 0;
    std::cout << "Player hit! Health: " << health << std::endl;
}


// Function To Load All Assets

void Player::loadAssets()
{
    // --- Load Textures for ALL 10 UNIQUE STATES ---
    if (!idleTexture.loadFromFile("../assets/idle.png"))
    { /* error */
    }
    if (!idleShootTexture.loadFromFile("../assets/idleShoot.png"))
    { /* error */
    }

    if (!runTexture.loadFromFile("../assets/run.png"))
    { /* error */
    }
    if (!runShootTexture.loadFromFile("../assets/runShoot.png"))
    { /* error */
    }

    if (!jumpTexture.loadFromFile("../assets/jump.png"))
    { /* error */
    }
    if (!jumpShootTexture.loadFromFile("../assets/jumpShoot.png"))
    { /* error */
    }

    if (!crouchTexture.loadFromFile("../assets/crouch.png"))
    { /* error */
    }
    if (!crouchShootTexture.loadFromFile("../assets/crouchShoot.png"))
    { /* error */
    }

    if (!dashTexture.loadFromFile("../assets/dash.png"))
    { /* error */
    }
    if (!doubleJumpTexture.loadFromFile("../assets/doubleJump.png"))
    { /* error */
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
        // --- JUMP LOGIC (W or Up) ---
        if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up))
        {
            if (jumpCount < MAX_JUMPS)
            {
                velocity.y = -JUMP_SPEED;
                onGround = false;
                if (jumpCount > 0)
                    isDoubleJumping = true;
                jumpCount++;

                animFrame = 0;
                animTimer = 0.f;
            }
        }

        // --- DASH LOGIC (Shift) ---
        if (event.key.code == sf::Keyboard::LShift)
        {
            if (canDash && !isDashing)
            {
                isDashing = true;
                canDash = false;
                dashTimeLeft = DASH_DURATION;
                dashCooldownTime = DASH_COOLDOWN;

                animFrame = 0;
                animTimer = 0.f;
            }
        }
    }
}

void Player::update(float deltaTime, std::vector<Bullet> &bullets, sf::Texture &bulletTexture, std::vector<GameObject> &gameObjects)
{
    // --- 1. INPUT TRACKING ---
    bool isMoving = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    bool wantsCrouch = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool wantsShoot = sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    // 1.1 Track Facing Direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        facingDirection = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        facingDirection = -1;

    // --- 2. MOVEMENT & PHYSICS LOGIC ---

    // 2.1 DASH LOGIC
    if (isDashing)
    {
        velocity.x = facingDirection * DASH_SPEED;
        velocity.y = 0; 

        dashTimeLeft -= deltaTime;
        if (dashTimeLeft <= 0)
        {
            isDashing = false;
            velocity.x = 0;
        }
    }
    // 2.2 NORMAL BEHAVIOR 
    else
    {
        // A. Handle Crouch
        if (onGround && wantsCrouch)
        {
            isCrouching = true;
            velocity.x = 0; 
        }
        else
        {
            isCrouching = false;

            // B. Handle Walking 
            velocity.x = 0.f; 

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                velocity.x = -MOVE_SPEED;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                velocity.x = MOVE_SPEED;
        }

        // C. Handle Gravity
        if (!onGround)
        {
            velocity.y += GRAVITY * deltaTime;
        }

        if (shootTimer > 0.0f) {
            shootTimer -= deltaTime;
        }

        // D. Handle Shooting
        static bool isFiring = false;
        if (wantsShoot)
        {
            if (shootTimer <= 0.0f)
            {
                float direction = (facingDirection > 0) ? 1.0f : -1.0f;
                sf::Vector2f spawnPos = sprite.getPosition();
                spawnPos.y -= sprite.getGlobalBounds().height / 2.0f;
                bullets.push_back(Bullet(bulletTexture, spawnPos, direction));
                shootTimer = 0.2f;
            }
        }
    }

    // --- DASH COOLDOWN ---
    if (!canDash)
    {
        dashCooldownTime -= deltaTime;
        if (dashCooldownTime <= 0)
            canDash = true;
    }

    isShooting = wantsShoot;

    // --- 3. SPRITE FLIPPING ---
    if (facingDirection > 0)
        sprite.setScale(3.f, 3.f);
    else if (facingDirection < 0)
        sprite.setScale(-3.f, 3.f);

    // --- 4. ANIMATION STATE MACHINE ---
    const sf::Texture *oldTexture = sprite.getTexture();
    textureSwitchTimer += deltaTime;

    if (textureSwitchTimer > 0.1f)
    {
        textureSwitchTimer = 0.f;
        const sf::Texture *newTexture = oldTexture;

        // Priority 1: Dashing
        if (isDashing)
        {
            newTexture = &dashTexture;
            currentFrameCount = DASH_FRAMES;
        }
        // Priority 2: Air
        else if (!onGround)
        {
            if (isDoubleJumping)
            {
                newTexture = &doubleJumpTexture;
                currentFrameCount = DOUBLE_JUMP_FRAMES;
            }
            else
            {
                newTexture = isShooting ? &jumpShootTexture : &jumpTexture;
                currentFrameCount = JUMP_FRAMES;
            }
        }
        // Priority 3: Crouch
        else if (isCrouching)
        {
            newTexture = isShooting ? &crouchShootTexture : &crouchTexture;
            currentFrameCount = CROUCH_FRAMES;
        }
        // Priority 4: Run
        else if (velocity.x != 0.f)
        {
            newTexture = isShooting ? &runShootTexture : &runTexture;
            currentFrameCount = RUN_FRAMES;
        }
        // Priority 5: Idle
        else
        {
            newTexture = isShooting ? &idleShootTexture : &idleTexture;
            currentFrameCount = IDLE_FRAMES;
        }

        if (oldTexture != newTexture)
        {
            sprite.setTexture(*newTexture);
            animFrame = 0;
            animTimer = 0.f;
        }
    }

    // --- 5. APPLY MOVEMENT ---
    sprite.move(velocity * deltaTime);

    // --- 6. COLLISION & GROUND CHECKS ---
    sf::FloatRect playerBounds = getBounds();
    
    // Reset onGround for this frame
    onGround = false; 

    // 6.1 Standard Collision (Blocks)
    for (const auto &obj : gameObjects)
    {
        sf::FloatRect objBounds = obj.getBounds();
        if (playerBounds.intersects(objBounds))
        {
            float overlapLeft = (playerBounds.left + playerBounds.width) - objBounds.left;
            float overlapRight = (objBounds.left + objBounds.width) - playerBounds.left;
            float overlapTop = (playerBounds.top + playerBounds.height) - objBounds.top;
            float overlapBottom = (objBounds.top + objBounds.height) - playerBounds.top;
            float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

            if (minOverlap == overlapTop && velocity.y > 0)
            {
                sprite.setPosition(sprite.getPosition().x, objBounds.top);
                velocity.y = 0.f;
                onGround = true;
                jumpCount = 0;
                isDoubleJumping = false;
            }
            else if (minOverlap == overlapBottom && velocity.y < 0)
            {
                sprite.setPosition(sprite.getPosition().x, objBounds.top + objBounds.height + playerBounds.height);
                velocity.y = 0.f;
            }
            else if (minOverlap == overlapLeft && velocity.x > 0)
            {
                sprite.setPosition(objBounds.left - playerBounds.width / 2.f, sprite.getPosition().y);
                velocity.x = 0.f;
            }
            else if (minOverlap == overlapRight && velocity.x < 0)
            {
                sprite.setPosition(objBounds.left + objBounds.width + playerBounds.width / 2.f, sprite.getPosition().y);
                velocity.x = 0.f;
            }
        }
    }

    // 6.2 Foot Sensor for Blocks (Fixes flickering on blocks)
    if (!onGround && velocity.y >= 0)
    {
        sf::FloatRect feetRect = getBounds();
        feetRect.top += feetRect.height; 
        feetRect.height = 2.0f;          
        
        for (const auto &obj : gameObjects)
        {
            if (feetRect.intersects(obj.getBounds()))
            {
                onGround = true;
                velocity.y = 0.f;
                jumpCount = 0;
                isDoubleJumping = false;
                break; 
            }
        }
    }

    // 6.3 Floor Boundary Check (The Fix for Base Ground)
    playerBounds = getBounds(); 
    float feetPosition = playerBounds.top + playerBounds.height;

    // Check if we are touching or below the floor
    if (feetPosition >= 950.f)
    {
        // 1. If falling, snap to floor
        if (velocity.y > 0)
        {
            float overlap = feetPosition - 950.f;
            sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - overlap);
            velocity.y = 0;
        }

        // 2. If we are on the floor (not moving up), set Grounded
        if (velocity.y >= 0)
        {
            onGround = true;
            jumpCount = 0;
            isDoubleJumping = false;
        }
    }

    // --- 7. ANIMATION FRAME UPDATES ---
    animTimer += deltaTime;
    jumpAnimTimer += deltaTime;

    if (!onGround && !isDoubleJumping && jumpAnimTimer >= JUMP_ANIM_SPEED)
    {
        jumpAnimTimer = 0.0f;
        if (animFrame < JUMP_FRAMES - 1)
            animFrame++;
    }

    if (animTimer >= ANIM_SPEED)
    {
        animTimer = 0.f;
        if (isCrouching && onGround)
        {
            if (animFrame < CROUCH_FRAMES - 2)
                animFrame++;
        }
        else
        {
            animFrame++;
            if (animFrame >= currentFrameCount)
                animFrame = 0;
        }
    }

    sprite.setTextureRect(sf::IntRect(animFrame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
}

// This just draws the sprite to the window
void Player::draw(sf::RenderWindow &window)
{
    window.draw(sprite);

    // sf::RectangleShape debugBox;
    // sf::FloatRect bounds = getBounds();
    // debugBox.setPosition(bounds.left, bounds.top);
    // debugBox.setSize(sf::Vector2f(bounds.width, bounds.height));
    // debugBox.setFillColor(sf::Color::Transparent);
    // debugBox.setOutlineColor(sf::Color::Red);
    // debugBox.setOutlineThickness(2);
    // window.draw(debugBox);
}

sf::FloatRect Player::getBounds() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // --- CONFIGURATION ---
    // 0.4f = Reduce width by 40% (Making player skinnier)
    float scaleX = 0.69f;
    // 0.1f = Reduce height by 10% (Removing empty space above head)
    float scaleY = 0.52f;

    // Calculate pixels to remove
    float widthRemove = bounds.width * scaleX;
    float heightRemove = bounds.height * scaleY;

    // 1. Shrink Width (Sides)
    // Move left side IN, and reduce total width
    bounds.left += widthRemove / 2.f;
    bounds.width -= widthRemove;

    // 2. Shrink Height (Top Only)
    // Move top DOWN. Important: We do NOT change the bottom,
    // so the feet stay on the ground!
    bounds.top += heightRemove;
    bounds.height -= heightRemove;

    return bounds;
}
