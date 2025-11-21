#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>

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
        if (!healthTexture.loadFromFile("health_bar.png")) {
            throw std::runtime_error("Could not load health_spritesheet.png");
        }

        // Assuming the sheet is 4 frames stacked vertically
        frameWidth  = healthTexture.getSize().x;
        frameHeight = healthTexture.getSize().y / 4;

        healthSprite.setTexture(healthTexture);
        healthSprite.setPosition(20.f, 20.f);

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

    void draw(sf::RenderWindow& window) {
        window.draw(healthSprite);
    }
};

#endif
