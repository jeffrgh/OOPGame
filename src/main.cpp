#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "mainmenu.h" // K.Tridev Karthik MainMenu File.
// Game State For Main Menu
enum class Main_Menu{
    Nothing,
    Start,
    Exit
};

int main()
{
    // Setup Window and Background 
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

    sf::Font menuFont;
    if (!menuFont.loadFromFile("MainMenuFont.ttf")) // Or any font file you have
    {
        std::cerr << "Error loading MainMenuFont.ttf" << std::endl;
        return -1;
    }

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