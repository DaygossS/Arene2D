#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class TrailSystem {
public:
    TrailSystem(int width, int height, int tileSize, float thickness);

    void addTrail(sf::Vector2f position, sf::Color color);
    bool checkCollision(const sf::FloatRect& bounds, const sf::Vector2f& velocity) const;
    void draw(sf::RenderWindow& window);
    void reset();

private:
    float m_thickness;

    // ORDRE CRITIQUE ICI : m_texture AVANT m_sprite
    sf::RenderTexture m_texture;
    sf::Sprite m_sprite;

    sf::Image m_mask;
    sf::RectangleShape m_brush;
};