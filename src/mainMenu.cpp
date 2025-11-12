#include "MainMenu.h"
#include <iostream>

// Constructor implementation
MainMenu::MainMenu(sf::Font &font)
{
    // Note: We use the font that was passed in
    //  --- Set up the text objects ---
    titleText.setFont(font);
    titleText.setString("MY GAME");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(300, 100);

    startButton.setFont(font);
    startButton.setString("Start Game");
    startButton.setCharacterSize(30);
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition(300, 300);

    exitButton.setFont(font);
    exitButton.setString("Exit");
    exitButton.setCharacterSize(30);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setPosition(300, 400);
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
    // Just draw all our text objects to the window
    window.draw(titleText);
    window.draw(startButton);
    window.draw(exitButton);
}
