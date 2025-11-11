#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

int main()
{
    // Setup Window and Background
    sf::RenderWindow window(sf::VideoMode(1536, 1024), "Extraction");
    window.setFramerateLimit(30);

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    if (!backgroundTexture.loadFromFile("../assets/back.png"))
    {
        std::cerr << "Error loading background.png" << std::endl;
        return -1;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Create the Player
    Player myPlayer;
    myPlayer.loadAssets();

    sf::Clock gameClock;

    // The Game Loop
    while (window.isOpen())
    {
        float deltaTime = gameClock.restart().asSeconds();

        // Handle Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            myPlayer.handleEvents(event);
        }

        // Just tell the player to update itself!
        myPlayer.update(deltaTime);

        // Drawing frame by frame
        window.clear();
        window.draw(backgroundSprite);
        myPlayer.draw(window); // Tell the player to draw itself
        window.display();
    }

    return 0;
}