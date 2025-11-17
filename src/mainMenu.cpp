#include "mainMenu.h"
#include <iostream>

// Constructor implementation
MainMenu::MainMenu(sf::Font &font)
{

    if (!bgTexture.loadFromFile("../assets/MainMenu.png"))
    {
        std::cerr << "Error loading ../assets/MainMenu.png" << std::endl;
    }
    bgSprite.setTexture(bgTexture);
    // Note: We use the font that was passed in
    //  --- Set up the text objects ---
    creditsText.setFont(font);
    creditsText.setString("Presented To You By:\nAayush, Tridev, Jeffrey, Dhiyanesh");
    creditsText.setCharacterSize(20);
    creditsText.setFillColor(sf::Color::White);
    creditsText.setPosition(1200 , 950);

    titleText.setFont(font);
    titleText.setString("The Extraction");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(100 , 100);

    startButton.setFont(font);
    startButton.setString("Start Game");
    startButton.setCharacterSize(40);
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition(120, 300);

    exitButton.setFont(font);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(40);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setPosition(120, 400);
}

// Event handling implementation
Menu_Result MainMenu::handleEvent(sf::Event &event, sf::Vector2f mousePos)
{
    // --- 1. Handle Hover Effect ---
    // We do this first, every time, based on mouse position
    startButton.setFillColor(sf::Color::White);
    exitButton.setFillColor(sf::Color::White);

    if (startButton.getGlobalBounds().contains(mousePos))
    {
        startButton.setFillColor(sf::Color::Red);
    }
    if (exitButton.getGlobalBounds().contains(mousePos))
    {
        exitButton.setFillColor(sf::Color::Red);
    }

    // --- 2. Handle Clicks ---
    // We only check for clicks if a MouseButtonPressed event happened
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (startButton.getGlobalBounds().contains(mousePos))
            {
                std::cout << "Start button clicked!" << std::endl;
                return Menu_Result::Start; // Tell main.cpp to start the game
            }

            if (exitButton.getGlobalBounds().contains(mousePos))
            {
                std::cout << "Exit button clicked!" << std::endl;
                return Menu_Result::Exit; // Tell main.cpp to exit
            }
        }
    }

    // If no button was clicked, return Nothing
    return Menu_Result::Nothing;
}

// Drawing implementation
void MainMenu::draw(sf::RenderWindow &window)
{

    window.draw(bgSprite);
    // Just draw all our text objects to the window
    window.draw(titleText);
    window.draw(creditsText);
    window.draw(startButton);
    window.draw(exitButton);
}
