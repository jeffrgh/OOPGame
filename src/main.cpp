#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "player.h"
#include "mainMenu.h"
#include "pauseMenu.h"
#include "movementTutorial.h"

#include "shooting.h" // Shooting files yet to be commited

enum class GameState
{
    MainMenu,
    Playing,
    PAUSED,
    Exiting
};

sf::Texture bulletTexture;
std::vector<Bullet> bulletList;

int main()
{
    // Setup Window and Background
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Extraction Game Thing");
    window.setFramerateLimit(30);

    // Adding camera here
    sf::View camera(sf::FloatRect(0.f, 0.f, 1536.f, 1024.f));

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
    if (!bulletTexture.loadFromFile("../assets/Bullet.jpg")) // (Needs "Bullet.png")
    {
        std::cerr << "Error loading Bullet.png" << std::endl;
        return -1;
    }

    // Create the Main Menu
    MainMenu mainMenu(menuFont);
    PauseMenu pauseMenu(menuFont);
    movementTutorial move(menuFont);
    GameState currentGameState = GameState::MainMenu;

    // Create the Player
    Player myPlayer;
    myPlayer.loadAssets();

    sf::Clock gameClock;
    bool showTutorial = false;
    sf::Clock tutorialTimer;

    // The Game Loop
    while (window.isOpen())
    {
        float deltaTime = 0.0f;
        if (currentGameState == GameState::Playing)
        {
            deltaTime = gameClock.restart().asSeconds();
        }
        else
        {
            gameClock.restart(); // keep restarting the time so it doesnt build up....
        }
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
                    showTutorial = true;
                    tutorialTimer.restart();
                }

                else if (result == Menu_Result::Exit)
                {
                    window.close();
                }
            }
            else if (currentGameState == GameState::Playing)
            {
                myPlayer.handleEvents(event);
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    currentGameState = GameState::PAUSED;
                }
                else if (event.type == sf::Event::KeyPressed && (event.key.code == sf ::Keyboard ::W || event.key.code == sf ::Keyboard ::A || event.key.code == sf ::Keyboard ::S || event.key.code == sf ::Keyboard ::D))
                {
                    showTutorial = false;
                }
            }
            else if (currentGameState == GameState::PAUSED)
            {
                window.setView(window.getDefaultView());
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                Pause_Result result = pauseMenu.handleEvent(event, mousePos);

                if (result == Pause_Result::Resume)
                {
                    currentGameState = GameState::Playing;
                }
                else if (result == Pause_Result::QuitToMenu)
                {
                    currentGameState = GameState::MainMenu;
                    camera.setCenter(1920.f / 2.f, 1080.f / 2.f);
                }
            }
        }

        // Update based on game state
        if (currentGameState == GameState::Playing)
        {
            myPlayer.update(deltaTime, bulletList, bulletTexture);
            for (int i = 0; i < bulletList.size(); i++)
            {
                bulletList[i].update(deltaTime);
            }

            if (showTutorial && tutorialTimer.getElapsedTime().asSeconds() > 10.0f)
            {
                showTutorial = false;
            }
        }

        // Drawing frame by frame
        window.clear();

        if (currentGameState == GameState::MainMenu)
        {
            window.setView(window.getDefaultView());
            mainMenu.draw(window);
        }
        else if (currentGameState == GameState::Playing)
        {
            window.setView(camera);
            window.draw(backgroundSprite);
            myPlayer.draw(window); // Tell the player to draw itself

            for (int i = 0; i < bulletList.size(); i++)
            {
                bulletList[i].draw(window);
            }

            if (showTutorial)
            {
                window.setView(window.getDefaultView());
                move.draw(window);
            } // Tell the player to draw itself
        }

        else if (currentGameState == GameState::PAUSED)
        {
            // Draw the "frozen" game world first
            window.setView(camera);
            window.draw(backgroundSprite);
            myPlayer.draw(window);
            window.setView(window.getDefaultView());
            pauseMenu.draw(window);
        }

        window.display();
    }

    return 0;
}
