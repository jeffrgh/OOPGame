#include <iostream>
#include "player.h"
#include "Shooting.h" // Assuming this defines the Bullet structure

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
    // shootTimer = 0.0f; // Uncomment if you are using shootTimer
    jumpAnimTimer = 0.0f;
}

// Function To Load All Assets
void Player::loadAssets()
{
    // --- Load Textures for ALL 10 UNIQUE STATES ---
    if (!idleTexture.loadFromFile("../assets/idle.png")) { /* error */ }
    if (!idleShootTexture.loadFromFile("../assets/idleShoot.png")) { /* error */ }

    if (!runTexture.loadFromFile("../assets/run.png")) { /* error */ }
    if (!runShootTexture.loadFromFile("../assets/runShoot.png")) { /* error */ }
    
    if (!jumpTexture.loadFromFile("../assets/jump.png")) { /* error */ }
    if (!jumpShootTexture.loadFromFile("../assets/jumpShoot.png")) { /* error */ }
    
    if (!crouchTexture.loadFromFile("../assets/crouch.png")) { /* error */ }
    if (!crouchShootTexture.loadFromFile("../assets/crouchShoot.png")) { /* error */ }

    if (!dashTexture.loadFromFile("../assets/dash.png")) { /* error */ }
    if (!doubleJumpTexture.loadFromFile("../assets/doubleJump.png")) { /* error */ }
    
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
                if (jumpCount > 0 ) isDoubleJumping = true;
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

void Player::update(float deltaTime, std::vector<Bullet>& bulletList, sf::Texture& bulletTexture)
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
    
    // 2.1 Dash Physics
    if (isDashing)
    {
        velocity.x = facingDirection * DASH_SPEED;
        velocity.y = 0; 
        dashTimeLeft -= deltaTime;
        if (dashTimeLeft <= 0) {
            isDashing = false;
            velocity.x = 0;
        }
    }
    else // Normal Behavior
    {
        // 2.2 Crouch Logic (Fix for Run->Crouch transition)
        if (onGround && wantsCrouch)
        {
            isCrouching = true;
            velocity.x = 0; // Force stop horizontal movement when crouching
        }
        else
        {
            isCrouching = false;
            
            // 2.3 Run/Walk Logic
            velocity.x = 0.f;
            if (isMoving && !isCrouching) // Only move if not crouching
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    velocity.x = -MOVE_SPEED;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)|| sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    velocity.x = MOVE_SPEED;
            }
        }
    
        // 2.4 Gravity Logic
        if (!onGround)
        {
            velocity.y += GRAVITY * deltaTime;
        }
    }

    // 2.5 Dash Cooldown
    if (!canDash)
    {
        dashCooldownTime -= deltaTime;
        if (dashCooldownTime <= 0) canDash = true;
    }
    
    // 2.6 Shooting State
    isShooting = wantsShoot; 

    // --- 3. SPRITE FLIPPING ---
    if (facingDirection > 0)
        sprite.setScale(3.f, 3.f);
    else if (facingDirection < 0)
        sprite.setScale(-3.f, 3.f);

    // --- 4. ANIMATION STATE MACHINE (10 States) ---
    const sf::Texture *oldTexture = sprite.getTexture();
    textureSwitchTimer += deltaTime;
    
    if (textureSwitchTimer > 0.1f)
    {
        textureSwitchTimer = 0.f;
        
        const sf::Texture* newTexture = oldTexture;
        
        // 1. DASH (Highest Priority)
        if (isDashing)
        {
            newTexture = &dashTexture;
            currentFrameCount = DASH_FRAMES;
        }
        // 2. JUMP / AIRBORNE
        else if (!onGround) 
        {
            if (isDoubleJumping)
            {
                newTexture = &doubleJumpTexture;
                currentFrameCount = DOUBLE_JUMP_FRAMES;
            }
            else // Single Jump
            {
                newTexture = isShooting ? &jumpShootTexture : &jumpTexture;
                currentFrameCount = JUMP_FRAMES;
            }
        }
        // 3. CROUCH
        else if (isCrouching)
        {
            newTexture = isShooting ? &crouchShootTexture : &crouchTexture;
            currentFrameCount = CROUCH_FRAMES;
        }
        // 4. RUN
        else if (velocity.x != 0.f)
        {
            newTexture = isShooting ? &runShootTexture : &runTexture;
            currentFrameCount = RUN_FRAMES;
        }
        // 5. IDLE (Default)
        else 
        {
            newTexture = isShooting ? &idleShootTexture : &idleTexture;
            currentFrameCount = IDLE_FRAMES;
        }

        // Apply new texture only if it changed
        if (oldTexture != newTexture)
        {
            sprite.setTexture(*newTexture);
            animFrame = 0; // Reset animation frame
            animTimer = 0.f;
        }
    }

    // --- 5. APPLY MOVEMENT ---
    sprite.move(velocity * deltaTime);

    // --- 6. GROUND COLLISION CHECK (Boundary Check) ---
    if (sprite.getPosition().y >= 950.f)
    {
        if (velocity.y > 0) 
        {
            sprite.setPosition(sprite.getPosition().x, 950.f);
            onGround = true;
            velocity.y = 0;
            jumpCount = 0;
            isDoubleJumping = false;
        }
    }

    // --- 7. ANIMATION FRAME UPDATE (THE CROUCH HOLD FIX) ---
 animTimer += deltaTime;
    jumpAnimTimer += deltaTime; // Timer update placed correctly outside the animTimer check

    // Check jump timer independently to manage slow jump frames
    if (!onGround && !isDoubleJumping && jumpAnimTimer >= JUMP_ANIM_SPEED)
    {
        jumpAnimTimer = 0.0f;
        // Only advance the single jump frame if we haven't reached the final 'hold' frame.
        if (animFrame < JUMP_FRAMES - 1)
        {
            animFrame++;
        }
    }
    
    // Check main animation timer for all other states (Crouch, Idle, Run, Dash, DoubleJump)
    if (animTimer >= ANIM_SPEED)
    {
        animTimer = 0.f;
        
        // **CROUCH / JUMP HOLD LOGIC**
        if (isCrouching && onGround) 
        {
            // Crouch: Hold on the final frame (index 3).
            if (animFrame < CROUCH_FRAMES - 2)
            {
                animFrame++; 
            }
            // If the player releases Ctrl, isCrouching becomes false, and the next frame will be Idle/Run (Point 4).
        }
        else if (isDoubleJumping) 
        {
            // Double Jump: Full loop
            animFrame++;
            if (animFrame >= currentFrameCount)
            {
                animFrame = 0; 
            }
        }
        else if (!onGround)
        {
            // Single Jump: Handled by the jumpAnimTimer above. We do nothing here to prevent it from cycling faster.
        }
        // **NORMAL LOOPING LOGIC (Covers Idle, Run, Dash)**
        else 
        {
            animFrame++;
            if (animFrame >= currentFrameCount)
            {
                animFrame = 0;
            }
        }
    }
    // --- 8. UPDATE TEXTURE RECT ---
    sprite.setTextureRect(sf::IntRect(animFrame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
}

// This just draws the sprite to the window
void Player::draw(sf::RenderWindow &window)
{
    window.draw(sprite); // Draw MAIN Sprite.
}


