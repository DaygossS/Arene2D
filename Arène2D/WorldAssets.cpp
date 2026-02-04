#include "WorldAssets.hpp"

sf::Texture createTronTexture(int tileSize, int cols) {
    sf::RenderTexture rt;

   
    rt.resize({ static_cast<unsigned int>(tileSize * cols), static_cast<unsigned int>(tileSize) });
    rt.clear(sf::Color::Black);

    sf::RectangleShape rect(sf::Vector2f((float)tileSize, (float)tileSize));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(-2.f);

    
    rect.setPosition({ 0.f, 0.f });
    rect.setOutlineColor(sf::Color(0, 50, 100));
    rect.setOutlineThickness(-1.f);
    rt.draw(rect);

    
    rect.setPosition({ (float)tileSize, 0.f });
    rect.setOutlineColor(sf::Color::Cyan);
    rect.setOutlineThickness(-3.f);
    rect.setFillColor(sf::Color::Cyan);
    rt.draw(rect);

    
    rect.setPosition({ (float)tileSize * 2, 0.f });
    rect.setOutlineColor(sf::Color(255, 100, 0));
    rect.setOutlineThickness(-3.f);
    rect.setFillColor(sf::Color(255, 100, 0, 50));
    rt.draw(rect);


    rect.setPosition({ (float)tileSize * 3, 0.f });
    rect.setOutlineColor(sf::Color::Magenta);
    rect.setOutlineThickness(-3.f);
    rect.setFillColor(sf::Color(255, 0, 255, 50));
    rt.draw(rect);

    rt.display();
    return rt.getTexture();
}