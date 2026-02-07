#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include "WorldAssets.hpp"
#include "Player.hpp"
#include "TrailSystem.hpp" // On inclut notre nouvelle classe

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 5;

// Fonction utilitaire map (inchangée)
void openMap(std::vector<int>& mapData, std::vector<int>& colData, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    int val; std::string tempStr; size_t count = 0;
    while (count < mapData.size() && file >> val) { mapData[count++] = val; }
    file.clear(); file >> tempStr; count = 0;
    while (count < colData.size() && file >> val) { colData[count++] = val; }
}

// Fonction collision map (inchangée)
bool isCollidingWithMap(const sf::FloatRect& bounds, const std::vector<int>& collisions) {
    sf::Vector2f points[4] = {
        { bounds.position.x, bounds.position.y },
        { bounds.position.x + bounds.size.x, bounds.position.y },
        { bounds.position.x, bounds.position.y + bounds.size.y },
        { bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y }
    };
    for (int i = 0; i < 4; i++) {
        int gridX = (int)(points[i].x / TILE_SIZE);
        int gridY = (int)(points[i].y / TILE_SIZE);
        if (gridX >= 0 && gridX < MAP_WIDTH && gridY >= 0 && gridY < MAP_HEIGHT) {
            if (collisions[gridY * MAP_WIDTH + gridX] == 1) return true;
        }
        else { return true; }
    }
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "TRON GAME");
    window.setFramerateLimit(60);

    // --- Assets ---
    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite mapSprite(tileset);

    sf::Texture playerZ, playerS, playerQ, playerD;
    if (!playerZ.loadFromFile("../Assets/Player/playerZ.png")) std::cerr << "Erreur Z" << std::endl;
    if (!playerS.loadFromFile("../Assets/Player/playerS.png")) std::cerr << "Erreur S" << std::endl;
    if (!playerQ.loadFromFile("../Assets/Player/playerQ.png")) std::cerr << "Erreur Q" << std::endl;
    if (!playerD.loadFromFile("../Assets/Player/playerD.png")) std::cerr << "Erreur D" << std::endl;
    PlayerTextures playerAssets = { playerZ, playerS, playerQ, playerD };

    // --- Initialisation du Monde ---
    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);
    openMap(map, collisions, "niveau2.txt");

    // --- Initialisation des Objets ---
    Player player(100.f, 300.f, playerAssets);

    // NOUVEAU : On crée le système de traînée ici (largeur, hauteur, taille_tuile, épaisseur)
    TrailSystem trailSystem(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 6.f);

    sf::Clock clock;

    // Debug
    bool showHitbox = false;
    sf::RectangleShape debugBox;
    debugBox.setFillColor(sf::Color::Transparent);
    debugBox.setOutlineColor(sf::Color::Red);
    debugBox.setOutlineThickness(1.f);

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) window.close();
                if (key->scancode == sf::Keyboard::Scancode::F1) showHitbox = !showHitbox;
            }
        }

        player.handleInput();
        player.update(deltaTime);

        // Hitbox calculée
        float hitboxSize = 20.f;
        sf::Vector2f pPos = player.getPosition();
        sf::FloatRect playerBounds({ pPos.x - hitboxSize / 2.f, pPos.y - hitboxSize / 2.f }, { hitboxSize, hitboxSize });

        bool isDead = false;

        // 1. Collision Mur Map
        if (isCollidingWithMap(playerBounds, collisions)) {
            isDead = true;
        }
        // 2. Collision Traînée (via notre nouvelle classe)
        else if (trailSystem.checkCollision(playerBounds, player.getVelocity())) {
            isDead = true;
        }

        if (isDead) {
            player.reset(100.f, 300.f);
            trailSystem.reset(); 
            continue;
        }

        
        trailSystem.addTrail(player.getPosition(), sf::Color::White);

        // --- Rendu ---
        window.clear(sf::Color::Black);

        // 1. Sol
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

        // 2. Traînées (toutes)
        trailSystem.draw(window);

        // 3. Joueur
        player.draw(window);

        if (showHitbox) {
            debugBox.setSize(playerBounds.size);
            debugBox.setPosition(playerBounds.position);
            window.draw(debugBox);
        }

        window.display();
    }
    return 0;
}