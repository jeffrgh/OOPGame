#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"
#include "mainMenu.h" // K.Tridev Karthik MainMenu File.

enum class GameState
{
    MainMenu,
    Playing,
    Exiting
};

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

    sf::Font menuFont;
    if (!menuFont.loadFromFile("../assets/mainMenuFont.ttf")) // Or any font file you have
    {
        std::cerr << "Error loading mainMenuFont.ttf" << std::endl;
        return -1;
    }

    // Create the Main Menu
    MainMenu mainMenu(menuFont);
    GameState currentGameState = GameState::MainMenu;

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

            if (currentGameState == GameState::MainMenu)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                Menu_Result result = mainMenu.handleEvent(event, mousePos);
                if (result == Menu_Result::Start)
                {
                    currentGameState = GameState::Playing;
                }
                else if (result == Menu_Result::Exit)
                {
                    window.close();
                }
            }
            else if (currentGameState == GameState::Playing)
            {
                myPlayer.handleEvents(event);
            }
        }

        // Update based on game state
        if (currentGameState == GameState::Playing)
        {
            myPlayer.update(deltaTime);
        }

        // Drawing frame by frame
        window.clear();
        window.draw(backgroundSprite);

        if (currentGameState == GameState::MainMenu)
        {
            mainMenu.draw(window);
        }
        else if (currentGameState == GameState::Playing)
        {
            myPlayer.draw(window); // Tell the player to draw itself
        }

        window.display();
    }

    return 0;
}