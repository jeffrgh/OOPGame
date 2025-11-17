#pragma once

#include <SFML/Graphics.hpp>

// 1. Define the results this menu can return
enum class Pause_Result
{
    Nothing,
    Resume,
    QuitToMenu
};

class PauseMenu
{
public:
    // Constructor (uses the same font)
    PauseMenu(sf::Font& font);

    // Public functions that main.cpp will call
    Pause_Result handleEvent(sf::Event& event, sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);

private:
    // Private variables for this class
    sf::Text pauseTitle;
    sf::Text resumeButton;
    sf::Text quitButton;

    // A background shape to dim the screen
    sf::RectangleShape backgroundDim;
};