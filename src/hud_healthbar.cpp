// #include <SFML/Graphics.hpp>
// #include "HUD.hpp"

// int main() {
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Animated HUD Health Bar");

//     HUD hud(100.f);  // max HP = 100

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         // Test input
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//             hud.damage(1);   // hold A to damage

//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//             hud.heal(1);     // hold D to heal

//         window.clear(sf::Color::Black);
//         hud.draw(window);
//         window.display();
//     }

//     return 0;
// }

