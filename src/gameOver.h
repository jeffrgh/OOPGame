#pragma once
#include <SFML/Graphics.hpp>

enum class GameOver_Result
{
    Nothing,
    Restart,
    Exit
};

class GameOver
{
public:
    GameOver(sf::Font &font);
    GameOver_Result handleEvent(sf::Event &event, sf::Vector2f mousePos);
    void draw(sf::RenderWindow &window);

private:
    sf::Text titleText;
    sf::Text restartButton;
    sf::Text exitButton;
    sf::RectangleShape backgroundDim;
};