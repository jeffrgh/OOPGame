#include "movementTutorial.h"
#include <iostream> // For std::cerr

// Constructor implementation
movementTutorial::movementTutorial(sf::Font& font) // <-- FIX 1: Changed name
{
    // --- 1. Set up the dim background ---
    backgroundDim.setSize(sf::Vector2f(1536, 1024)); // Your window's size
    backgroundDim.setFillColor(sf::Color(0, 0, 0, 150)); // Black, 150/255 opacity

    // --- 2. Load all key images ---
    // (Your png names are correct)
    if (!wKeyTexture.loadFromFile("../assets/keyboardW.png")) { std::cerr << "Error loading keyboardw.png\n"; }
    if (!aKeyTexture.loadFromFile("../assets/keyboardA.png")) { std::cerr << "Error loading keyboardA.png\n"; }
    if (!sKeyTexture.loadFromFile("../assets/keyboardS.png")) { std::cerr << "Error loading keyboardS.png\n"; }
    if (!dKeyTexture.loadFromFile("../assets/keyboardD.png")) { std::cerr << "Error loading keyboardD.png\n"; }

    wKeySprite.setTexture(wKeyTexture);
    aKeySprite.setTexture(aKeyTexture);
    sKeySprite.setTexture(sKeyTexture);
    dKeySprite.setTexture(dKeyTexture);
    
    // (Optional: Set scale for images)
    // wKeySprite.setScale(1.5f, 1.5f);
    // aKeySprite.setScale(1.5f, 1.5f);
    // sKeySprite.setScale(1.5f, 1.5f);
    // dKeySprite.setScale(1.5f, 1.5f);

    // --- 3. Set up the "MOVE" Title ---
    moveTitle.setFont(font);
    moveTitle.setCharacterSize(50);
    moveTitle.setFillColor(sf::Color::White);
    moveTitle.setString("MOVE");
    
    // --- 4. SET ALL POSITIONS (Manual X, Y) ---
    // Change these (x, y) values to move your items
    
    // Example: Centering the "MOVE" title
    moveTitle.setOrigin(moveTitle.getLocalBounds().width / 2.0f, moveTitle.getLocalBounds().height / 2.0f);
    moveTitle.setPosition(1536 / 2.0f, 400); // (Centered, 400px down)

    // Example: Placing keys in a WASD shape below the title
    wKeySprite.setPosition(748, 480);
    aKeySprite.setPosition(688, 540);
    sKeySprite.setPosition(748, 540);
    dKeySprite.setPosition(808, 540);
}


// Drawing implementation
void movementTutorial::draw(sf::RenderWindow& window) // <-- FIX 2: Changed name
{
    // Draw the dim background FIRST
    window.draw(backgroundDim);

    // Draw all your items
    window.draw(moveTitle);
    window.draw(wKeySprite);
    window.draw(aKeySprite);
    window.draw(sKeySprite);
    window.draw(dKeySprite);
}