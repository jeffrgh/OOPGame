#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class HUD {
private:
    float maxHealth;
    float currentHealth;

    sf::Texture healthTexture;
    sf::Sprite healthSprite;

    int frameWidth;
    int frameHeight;

public:
    HUD(float maxHealth) {
        this->maxHealth = maxHealth;
        this->currentHealth = maxHealth;

        // Load health bar sprite sheet
        if (!healthTexture.loadFromFile("../assets/health_bar.png")) {
            std::cerr << "Error: Could not load ../assets/health_bar.png" << std::endl;
        }

        // Assuming the sheet is 4 frames stacked vertically
        frameWidth  = healthTexture.getSize().x;
        frameHeight = healthTexture.getSize().y / 4;

        healthSprite.setTexture(healthTexture);
        
        // --- FIX 1: Position ---
        // Moved closer to the top-left corner
        healthSprite.setPosition(10.f, 10.f);
        
        // --- FIX 2: Scale ---
        // Reduced to 0.5f (Half size). 
        // Adjust this number if you want it smaller/bigger (e.g., 0.3f or 0.7f)
        healthSprite.setScale(0.25f, 0.25f); 

        updateHealthFrame();
    }

    void setHealth(float health) {
        currentHealth = std::max(0.0f, std::min(health, maxHealth));
        updateHealthFrame();
    }

    void damage(float amount) { setHealth(currentHealth - amount); }
    void heal(float amount)   { setHealth(currentHealth + amount); }

    void updateHealthFrame() {
        float percent = currentHealth / maxHealth;

        int frameIndex = 0;

        // Logic to switch frames based on health percentage
        if (percent > 0.75f)       frameIndex = 0; // full
        else if (percent > 0.50f)  frameIndex = 1; // ~75%
        else if (percent > 0.0f)   frameIndex = 2; // ~50%
        else                       frameIndex = 3; // empty

        healthSprite.setTextureRect(sf::IntRect(
            0,
            frameIndex * frameHeight,
            frameWidth,
            frameHeight
        ));
    }

    // --- FIX 3: Move with Camera (Sticky UI) ---
    void draw(sf::RenderWindow& window) {
        // 1. Save the current view (the game camera)
        sf::View currentView = window.getView();

        // 2. Switch to the "Default View" (Static UI coordinates)
        // This makes the HUD stick to the screen instead of the world
        window.setView(window.getDefaultView());

        // 3. Draw the HUD
        window.draw(healthSprite);

        // 4. Restore the game camera so the rest of the game draws correctly
        window.setView(currentView);
    }
};

#endif