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

int main() {
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "TRON GAME");
    window.setFramerateLimit(60);

    // 1. Charger les décors (Murs, Sol)
    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite mapSprite(tileset);

    // 2. Créer le "Calque de Trainée" (Canvas)
    sf::RenderTexture trailTexture;
    // On le crée de la taille exacte de la fenêtre/carte
    trailTexture.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) });
    // On le remplit de transparent au début
    trailTexture.clear(sf::Color::Transparent);

    // Le Sprite qui servira à afficher ce calque
    sf::Sprite trailDisplay(trailTexture.getTexture());

    // Chargement de la carte (Grille)
    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);
    openMap(map, collisions, "niveau3.txt");

    // Création du joueur (supposons qu'il fait 8px dans ton Player.cpp)
    Player player(100.f, 100.f);

    // Un petit rectangle qu'on utilisera comme "pinceau"
    sf::RectangleShape trailBrush;
    trailBrush.setFillColor(sf::Color::Cyan); // Couleur de la trainée

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

        // 2. Ensuite la trainée par dessus le sol
        window.draw(trailDisplay);

        // 3. Enfin le joueur par dessus tout
        player.draw(window);

        window.display();
    }
    return 0;
}