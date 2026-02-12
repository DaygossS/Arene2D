#include "CollisionManager.hpp"

namespace CollisionManager {

    sf::FloatRect getHitbox(const sf::Vector2f& position, float size) {
        return sf::FloatRect(
            { position.x - size / 2.f, position.y - size / 2.f },
            { size, size }
        );
    }

    bool checkMapCollision(const sf::FloatRect& bounds, const std::vector<int>& collisions, int mapWidth, int mapHeight, int tileSize) {
        sf::Vector2f points[4] = {
            { bounds.position.x, bounds.position.y },
            { bounds.position.x + bounds.size.x, bounds.position.y },
            { bounds.position.x, bounds.position.y + bounds.size.y },
            { bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y }
        };

        for (int i = 0; i < 4; i++) {
            int gridX = (int)(points[i].x / tileSize);
            int gridY = (int)(points[i].y / tileSize);

            if (gridX >= 0 && gridX < mapWidth && gridY >= 0 && gridY < mapHeight) {
                int index = gridY * mapWidth + gridX;
                if (collisions[index] == 1) return true;
            }
            else { return true; }
        }
        return false;
    }


    void drawHitbox(sf::RenderWindow& window, const sf::FloatRect& bounds, sf::Color color) {
        sf::RectangleShape debugShape;
        debugShape.setPosition(bounds.position);
        debugShape.setSize(bounds.size);
        debugShape.setFillColor(sf::Color::Transparent); // Intérieur vide
        debugShape.setOutlineColor(color);               // Bordure colorée
        debugShape.setOutlineThickness(1.f);
        window.draw(debugShape);
    }
}