#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "WorldAssets.hpp"
#include "Player.hpp"

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 5;

void openMap(std::vector<int>& mapData, std::vector<int>& colData, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    int val;
    std::string tempStr;
    size_t count = 0;
    while (count < mapData.size() && file >> val) {
        mapData[count] = val;
        count++;
    }
    file.clear();
    file >> tempStr;
    count = 0;
    while (count < colData.size() && file >> val) {
        colData[count] = val;
        count++;
    }
}
bool isCollidingWithMap(const sf::FloatRect& bounds, const std::vector<int>& collisions) {
    
    sf::Vector2f points[4] = {
        { bounds.position.x, bounds.position.y },                                     // Haut-Gauche
        { bounds.position.x + bounds.size.x, bounds.position.y },                     // Haut-Droite
        { bounds.position.x, bounds.position.y + bounds.size.y },                     // Bas-Gauche
        { bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y }      // Bas-Droite
    };

    
    for (int i = 0; i < 4; i++) {
        
        int gridX = (int)(points[i].x / TILE_SIZE);
        int gridY = (int)(points[i].y / TILE_SIZE);

        
        if (gridX >= 0 && gridX < MAP_WIDTH && gridY >= 0 && gridY < MAP_HEIGHT) {
            int index = gridY * MAP_WIDTH + gridX;

            
            if (collisions[index] == 1) {
                return true; 
            }
        }
        else {
            
            return true;
        }
    }

    return false; 
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "TRON GAME");
    window.setFramerateLimit(60);

    
    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite mapSprite(tileset);

    
    sf::RenderTexture trailTexture;
    
    trailTexture.resize({(unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE)});
    
    trailTexture.clear(sf::Color::Transparent);

    
    sf::Sprite trailDisplay(trailTexture.getTexture());

    
    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);
    openMap(map, collisions, "niveau5.txt");

    
    Player player(100.f, 300.f);

    
    sf::RectangleShape trailBrush;
    trailBrush.setFillColor(sf::Color::White); 

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();


        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }
        }

        player.handleInput();
        player.update(deltaTime);
        sf::FloatRect playerBounds(player.getPosition(), player.getSize());

        trailBrush.setSize({ player.getSize().x - 2,player.getSize().y - 2 });
        trailBrush.setPosition({player.getPosition().x + 1, player.getPosition().y + 1});

  
        trailTexture.draw(trailBrush);
        trailTexture.display();

        window.clear(sf::Color::Black);

     
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int index = y * MAP_WIDTH + x;
                int tID = map[index];
                if (tID >= TEXTURE_COLS) tID = 0;

                mapSprite.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });
                mapSprite.setTextureRect(sf::IntRect({ tID * TILE_SIZE, 0 }, { TILE_SIZE, TILE_SIZE }));
                window.draw(mapSprite);
            }
        }
        if (isCollidingWithMap(playerBounds, collisions)) {
            player = Player(100.f, 300.f);
            trailTexture.clear(sf::Color::Transparent);
        }


        
        window.draw(trailDisplay);

        
        player.draw(window);

        window.display();
    }
    return 0;
}