#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h" // --- Include your new Player blueprint ---

int main()
{
    // --- 1. Setup Window and Background ---
    sf::RenderWindow window(sf::VideoMode(1536, 1024), "Extraction");
    window.setFramerateLimit(30);

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    if (!backgroundTexture.loadFromFile("../assets/Back.png"))
    {
        std::cerr << "Error loading background.png" << std::endl;
        return -1;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // --- 2. Create the Player ---
    Player myPlayer; // This one line creates your player and runs its constructor
    myPlayer.loadAssets(); // Call our function to load the textures

    sf::Clock gameClock;

    // --- 3. The Game Loop ---
    while (window.isOpen())
    {
        float deltaTime = gameClock.restart().asSeconds();

        // --- Handle Events ---
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Pass the event to the player to handle
            myPlayer.handleEvents(event); 
        }

        // --- Update Logic ---
        // Just tell the player to update itself!
        myPlayer.update(deltaTime);

        // --- Drawing ---
        window.clear(); 
        window.draw(backgroundSprite);
        myPlayer.draw(window); // Tell the player to draw itself
        window.display();
    }
    
    return 0;
}