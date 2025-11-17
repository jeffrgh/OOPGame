#pragma once
#include <SFML/Graphics.hpp>

class movementTutorial 
{
public:
    movementTutorial(sf::Font& font); // <-- FIX 2: Changed constructor name
    void draw(sf::RenderWindow& window);

private:
    // The "MOVE" title
    sf::Text moveTitle;

    // The 4 key images
    sf::Texture wKeyTexture;
    sf::Sprite wKeySprite;
    sf::Texture aKeyTexture;
    sf::Sprite aKeySprite;
    sf::Texture sKeyTexture;
    sf::Sprite sKeySprite;
    sf::Texture dKeyTexture;
    sf::Sprite dKeySprite;

    // The dimming rectangle
    sf::RectangleShape backgroundDim;
};