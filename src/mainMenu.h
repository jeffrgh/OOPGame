#pragma once

#include <SFML/Graphics.hpp>

enum class Menu_Result
{
    Nothing,
    Start,
    Exit
};

class MainMenu
{
public:
    MainMenu(sf::Font &font); // used so we can put the font when we use it.
    Menu_Result handleEvent(sf::Event &event, sf::Vector2f mousePos);
    void draw(sf::RenderWindow &window);

private:
    sf::Text titleText;
    sf::Text creditsText;
    sf::Text startButton;
    sf::Text exitButton;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
};
