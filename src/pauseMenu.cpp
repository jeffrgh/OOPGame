#include "PauseMenu.h"
#include <iostream>

// Constructor implementation
PauseMenu::PauseMenu(sf::Font& font)
{
    // --- Set up the background dim ---
    // This shape will cover the whole screen
    backgroundDim.setSize(sf::Vector2f(1536, 1024)); // Use your window's size
    backgroundDim.setFillColor(sf::Color(0, 0, 0, 150)); // Black, 150/255 opacity

    //Paused Title.
    pauseTitle.setFont(font);
    pauseTitle.setString("PAUSED");
    pauseTitle.setCharacterSize(80);
    pauseTitle.setFillColor(sf::Color::White);
    pauseTitle.setOrigin(pauseTitle.getLocalBounds().width / 2.0f, pauseTitle.getLocalBounds().height / 2.0f);
    pauseTitle.setPosition(1536 / 2.0f, 250);

    // Setting Resume Button.
    resumeButton.setFont(font);
    resumeButton.setString("Resume");
    resumeButton.setCharacterSize(60);
    resumeButton.setFillColor(sf::Color::White);
    resumeButton.setOrigin(resumeButton.getLocalBounds().width / 2.0f, resumeButton.getLocalBounds().height / 2.0f);
    resumeButton.setPosition(1536 / 2.0f, 450);
    resumeButton.setPosition(1536 / 2.0f, 450);

    quitButton.setFont(font);
    quitButton.setString("Main Menu");
    quitButton.setCharacterSize(60);
    quitButton.setFillColor(sf::Color::White);
    quitButton.setOrigin(quitButton.getLocalBounds().width / 2.0f, quitButton.getLocalBounds().height / 2.0f);
    quitButton.setPosition(1536 / 2.0f, 650);
    quitButton.setPosition(1536 / 2.0f, 650);
}

// Event handling implementation
Pause_Result PauseMenu::handleEvent(sf::Event& event, sf::Vector2f mousePos)
{
    // --- 1. Handle Hover Effect ---
    resumeButton.setFillColor(sf::Color::White);
    quitButton.setFillColor(sf::Color::White);

    if (resumeButton.getGlobalBounds().contains(mousePos))
    {
        resumeButton.setFillColor(sf::Color::Red);
    }
    if (quitButton.getGlobalBounds().contains(mousePos))
    {
        quitButton.setFillColor(sf::Color::Red);
    }

    // --- 2. Handle Clicks ---
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (resumeButton.getGlobalBounds().contains(mousePos))
            {
                std::cout << "Resume button clicked!" << std::endl;
                return Pause_Result::Resume; // Tell main.cpp to resume
            }
            if (quitButton.getGlobalBounds().contains(mousePos))
            {
                std::cout << "Quit to Menu button clicked!" << std::endl;
                return Pause_Result::QuitToMenu; // Tell main.cpp to quit
            }
        }
    }

    // If no button was clicked, return Nothing
    return Pause_Result::Nothing;
}

// Drawing implementation
void PauseMenu::draw(sf::RenderWindow& window)
{
    // Draw the dim background FIRST
    window.draw(backgroundDim);

    // THEN draw the text on top
    window.draw(pauseTitle);
    window.draw(resumeButton);
    window.draw(quitButton);
}