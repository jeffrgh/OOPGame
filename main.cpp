#include <SFML/Graphics.hpp>
#include <iostream>

//Global players and physics variables

sf::Sprite playerSprite; //The player 
sf::Texture idleTexture; //Texture for Idle sprite
sf::Texture walkTexture; //Texture for walk sprite
sf::Texture jumpTexture; //Texture for jump sprite  
sf::Texture backgroundTexture;
sf::Sprite backgroundSprite;
sf::Clock gameClock; //The Game Clock


sf::Vector2f velocity(0.f, 0.f); // (x, y) speed
const float GRAVITY = 1000.f;    // pixels per second, pulling down
const float MOVE_SPEED = 400.f;  // pixels per second, for left/right
const float JUMP_SPEED = 500.f;  // pixels per second, for initial jump
bool onGround = true; // Checks if is on the ground


//Animation Sprite Variables
int animFrame = 0;            // Which frame (column) in that row
float animTimer = 0.f;        // A timer to know when to switch frames
const float ANIM_SPEED = 0.1f; // Switch frames every 0.1 seconds
const int FRAME_WIDTH = 128;   // Width of one sprite frame (ADJUST AS NEEDED)
const int FRAME_HEIGHT = 128;  // Height of one sprite frame (ADJUST AS NEEDED)
const int IDLE_FRAMES = 6;
const int RUN_FRAMES = 6;
const int JUMP_FRAMES = 2;
int currentFrameCount = IDLE_FRAMES;


int main()
{
    sf::RenderWindow window(sf::VideoMode(1536, 1024), "Extraction");

    if (!window.isOpen())
    {
        std::cerr << "ERROR: SFML Window failed to open." << std::endl;
        return -1; 
    }


    window.setFramerateLimit(60);

    if (!backgroundTexture.loadFromFile("Back.png"))
    {
        std::cerr << "Error loading background.png" << std::endl;
        return -1;
    }
    backgroundSprite.setTexture(backgroundTexture);

    if (!walkTexture.loadFromFile("Run.png"))
    {
        std::cerr << "Error loading character.png" << std::endl;
        return -1;
    }
    if (!idleTexture.loadFromFile("Idle.png"))
    {
        std::cerr << "Error loading character.png" << std::endl;
        return -1;
    }    
    playerSprite.setTexture(idleTexture);
    playerSprite.setPosition(100.f, 950.f);

    // --- NEW: Animation Setup ---
    // Set the origin to the CENTER of the sprite for correct flipping
    playerSprite.setOrigin(FRAME_WIDTH / 2.f, FRAME_HEIGHT); 
    
    // Set the initial "cookie cutter" to the first frame (idle)
    playerSprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
    // --- End Animation Setup ---
    playerSprite.setScale(3.f, 3.f);
    //edits


    while (window.isOpen())
    {
        float deltaTime = gameClock.restart().asSeconds(); // edits


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Up)
                {
                    if(onGround){// Player is on the ground, so let them jump
                        velocity.y = -JUMP_SPEED; // Shoot up (negative Y)
                        onGround = false;
                    }
            

                }
            }
        }


        
        velocity.x = 0.f; 

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            velocity.x = -MOVE_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            velocity.x = MOVE_SPEED;
        }

        // --- NEW: Sprite Flipping ---
        if (velocity.x > 0.f) // Moving right
            playerSprite.setScale(3.f, 3.f); // Normal scale
        else if (velocity.x < 0.f) // Moving left
            playerSprite.setScale(-3.f, 3.f); // Flipped horizontally
        // --- End Flipping ---

        // --- NEW: State Machine (Decide which animation to play) ---
        // (Assuming Row 0=Idle, Row 1=Walk, Row 2=Jump)

        if (velocity.x != 0.f)
        {
            playerSprite.setTexture(walkTexture); // Walking
        }
        else
        {
            playerSprite.setTexture(idleTexture); // Idle
        }


        // --- End State Machine ---


        if (!onGround)
        {
            velocity.y += GRAVITY * deltaTime;
        }

        // Check for ground collision (a simple floor)
        // We'll say the "floor" is at Y = 500
        if (playerSprite.getPosition().y > 950.f)
        {
            playerSprite.setPosition(playerSprite.getPosition().x, 950.f);
            onGround = true;
            velocity.y = 0; // Stop falling
        }

        animTimer += deltaTime;
        if (animTimer >= ANIM_SPEED)
        {
            animTimer = 0.f; 
            animFrame++; 
            
            // --- FIX 3: Use the correct frame count for the current animation ---
            if (animFrame >= currentFrameCount) // Use >= and the variable
            {
                animFrame = 0;
            }
        }
        
        // --- Update the "cookie cutter" ---
        playerSprite.setTextureRect(sf::IntRect(
            animFrame * FRAME_WIDTH,
            0, // animRow is 0 because each texture is its own file
            FRAME_WIDTH,              
            FRAME_HEIGHT              
        ));

        // --- 6. Final Update ---
        // Move the player based on the final velocity
        playerSprite.move(velocity * deltaTime);


        // --- 7. Drawing ---
        window.clear(); 
        // --- Draw the player ---
        window.draw(backgroundSprite);

        window.draw(playerSprite); 

        window.display();

        
    }
    return 0;

}