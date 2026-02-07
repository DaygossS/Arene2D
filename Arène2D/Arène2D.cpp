#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include "WorldAssets.hpp"
#include "Player.hpp"
#include "TrailSystem.hpp"
#include "CollisionManager.hpp" // <--- Indispensable maintenant

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 5;

// La fonction de chargement de map reste ici (c'est du chargement de données, pas de la physique)
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

    // --- 1. ASSETS ---
    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite mapSprite(tileset);

    sf::Texture playerZ, playerS, playerQ, playerD;
    if (!playerZ.loadFromFile("../Assets/Player/playerZ.png")) std::cerr << "Erreur Z" << std::endl;
    if (!playerS.loadFromFile("../Assets/Player/playerS.png")) std::cerr << "Erreur S" << std::endl;
    if (!playerQ.loadFromFile("../Assets/Player/playerQ.png")) std::cerr << "Erreur Q" << std::endl;
    if (!playerD.loadFromFile("../Assets/Player/playerD.png")) std::cerr << "Erreur D" << std::endl;

    PlayerTextures playerAssets = { playerZ, playerS, playerQ, playerD };

    // --- 2. MAP & ENTITÉS ---
    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);
    openMap(map, collisions, "niveau3.txt");

    Player player(100.f, 300.f, playerAssets);

    // Système de traînée
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

        // --- 3. PHYSIQUE VIA COLLISION MANAGER ---

        // A. On demande la Hitbox au Manager (Taille 20px)
        sf::FloatRect playerBounds = CollisionManager::getHitbox(player.getPosition(), 20.f);

        bool isDead = false;

        // B. On vérifie les murs via le Manager
        // (On lui passe la map, ses dimensions et la taille des tuiles)
        if (CollisionManager::checkMapCollision(playerBounds, collisions, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE)) {
            isDead = true;
            std::cout << "Mort: Mur Map" << std::endl;
        }
        // C. On vérifie la lumière via le TrailSystem
        else if (trailSystem.checkCollision(playerBounds, player.getVelocity())) {
            isDead = true;
            std::cout << "Mort: Trainee" << std::endl;
        }

        if (isDead) {
            player.reset(100.f, 300.f);
            trailSystem.reset();
            continue;
        }

        // --- 4. LOGIQUE JEU ---

        // Ajout de la traînée (Couleur Cyan)
        trailSystem.addTrail(player.getPosition(), sf::Color::White);

        // --- 5. RENDU ---
        window.clear(sf::Color::Black);

        // A. Map
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

        // B. Traînées
        trailSystem.draw(window);

        // C. Joueur
        player.draw(window);

        // D. Debug
        if (showHitbox) {
            debugBox.setSize(playerBounds.size);
            debugBox.setPosition(playerBounds.position);
            window.draw(debugBox);
        }

        window.display();
    }
    return 0;
}