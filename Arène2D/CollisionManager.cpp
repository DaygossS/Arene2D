#include "CollisionManager.hpp"

namespace CollisionManager {

    sf::FloatRect getHitbox(const sf::Vector2f& position, float size) {
        // SFML 3 : Constructeur {position}, {taille}
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

            // Vérification limites de la map
            if (gridX >= 0 && gridX < mapWidth && gridY >= 0 && gridY < mapHeight) {
                int index = gridY * mapWidth + gridX;
                // Si la case vaut 1, c'est un mur
                if (collisions[index] == 1) return true;
            }
            else {
                // Hors map = Collision
                return true;
            }
        }
        return false;
    }
}