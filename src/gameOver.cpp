#include "GameOver.h"
#include <iostream>

GameOver::GameOver(sf::Font &font)
{

    backgroundDim.setSize(sf::Vector2f(1920, 1080));
    backgroundDim.setFillColor(sf::Color(50, 0, 0, 200)); // Dark Red tint

    titleText.setFont(font);
    titleText.setString("GAME OVER");
    titleText.setCharacterSize(100);
    titleText.setFillColor(sf::Color::Red);
    titleText.setStyle(sf::Text::Bold);
    

    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    titleText.setPosition(1920 / 2.0f, 300);

    restartButton.setFont(font);
    restartButton.setString("Restart Game");
    restartButton.setCharacterSize(50);
    restartButton.setFillColor(sf::Color::White);
    
    sf::FloatRect restartRect = restartButton.getLocalBounds();
    restartButton.setOrigin(restartRect.left + restartRect.width/2.0f, restartRect.top + restartRect.height/2.0f);
    restartButton.setPosition(1920 / 2.0f, 500);


    exitButton.setFont(font);
    exitButton.setString("Exit to Desktop");
    exitButton.setCharacterSize(50);
    exitButton.setFillColor(sf::Color::White);
    
    sf::FloatRect exitRect = exitButton.getLocalBounds();
    exitButton.setOrigin(exitRect.left + exitRect.width/2.0f, exitRect.top + exitRect.height/2.0f);
    exitButton.setPosition(1920 / 2.0f, 600);
}

GameOver_Result GameOver::handleEvent(sf::Event &event, sf::Vector2f mousePos)
{

    restartButton.setFillColor(sf::Color::White);
    exitButton.setFillColor(sf::Color::White);

    if (restartButton.getGlobalBounds().contains(mousePos)) restartButton.setFillColor(sf::Color::Yellow);
    if (exitButton.getGlobalBounds().contains(mousePos))    exitButton.setFillColor(sf::Color::Yellow);

    // Click Handling
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        if (restartButton.getGlobalBounds().contains(mousePos)) return GameOver_Result::Restart;
        if (exitButton.getGlobalBounds().contains(mousePos))    return GameOver_Result::Exit;
    }

    return GameOver_Result::Nothing;
}

void GameOver::draw(sf::RenderWindow &window)
{
    window.draw(backgroundDim);
    window.draw(titleText);
    window.draw(restartButton);
    window.draw(exitButton);
}