#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace CollisionManager {

    // 1. Crée une hitbox carrée centrée sur la position avec une taille variable
    sf::FloatRect getHitbox(const sf::Vector2f& position, float size);

    // 2. Vérifie si la hitbox touche un mur (on passe les infos de la map en arguments)
    bool checkMapCollision(const sf::FloatRect& bounds, const std::vector<int>& collisions, int mapWidth, int mapHeight, int tileSize);

}