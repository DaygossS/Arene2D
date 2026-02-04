#include "WorldAssets.hpp"
#include <iostream>

sf::Texture createTronTexture(int tileSize, int cols) {
    sf::RenderTexture rt;

    static sf::Color Bleu = sf::Color(0, 50, 100);

    if (!rt.resize(sf::Vector2u{ (unsigned int)(tileSize * cols), (unsigned int)(tileSize) }))
        std::cerr << "La RenderTexture n'a pas pu être resize" << std::endl;
    rt.clear(sf::Color::Black);

    sf::RectangleShape grille(sf::Vector2f((float)tileSize, (float)tileSize));
    grille.setFillColor(sf::Color::Transparent);
    grille.setOutlineThickness(-2.f);

    
    grille.setPosition({ 0.f, 0.f });
    grille.setOutlineColor(Bleu);
    grille.setOutlineThickness(-1.f);
    rt.draw(grille);

    
    grille.setPosition({ (float)tileSize, 0.f });
    grille.setOutlineColor(sf::Color::Cyan);
    grille.setOutlineThickness(-3.f);
    grille.setFillColor(sf::Color::Cyan);
    rt.draw(grille);

    
    grille.setPosition({ (float)tileSize * 2, 0.f });
    grille.setOutlineColor(sf::Color(255, 100, 0));
    grille.setOutlineThickness(-3.f);
    grille.setFillColor(sf::Color(255, 100, 0, 50));
    rt.draw(grille);


    grille.setPosition({ (float)tileSize * 3, 0.f });
    grille.setOutlineColor(sf::Color::Magenta);
    grille.setOutlineThickness(-3.f);
    grille.setFillColor(sf::Color(255, 0, 255, 50));
    rt.draw(grille);

    rt.display();
    return rt.getTexture();
}