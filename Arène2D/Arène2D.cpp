#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include "WorldAssets.hpp"
#include "Player.hpp"
#include "TrailSystem.hpp"
#include "CollisionManager.hpp"
#include "ScoreSystem.hpp"

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 5;

// NOUVEAU : Hauteur de la bande noire pour le score (en pixels)
const int HUD_HEIGHT = 60;

void openMap(std::vector<int>& mapData, std::vector<int>& colData, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    int val; std::string tempStr; size_t count = 0;
    while (count < mapData.size() && file >> val) { mapData[count++] = val; }
    file.clear(); file >> tempStr; count = 0;
    while (count < colData.size() && file >> val) { colData[count++] = val; }
}

int main() {
    // 1. FENÊTRE AGRANDIE : On ajoute HUD_HEIGHT à la hauteur totale
    unsigned int totalWidth = MAP_WIDTH * TILE_SIZE;
    unsigned int gameHeight = MAP_HEIGHT * TILE_SIZE;
    unsigned int totalHeight = gameHeight + HUD_HEIGHT;

    sf::RenderWindow window(sf::VideoMode({ totalWidth, totalHeight }), "TRON GAME");
    window.setFramerateLimit(60);

    // --- CONFIGURATION DES CAMÉRAS (VUES) ---

    // Vue JEU : Elle regarde la map (taille normale), mais on l'affiche en bas
    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { (float)totalWidth, (float)gameHeight }));

    // Calcul du ratio pour le viewport (zone d'affichage à l'écran)
    // La zone de jeu commence après le HUD et prend le reste de la place
    float viewportY = (float)HUD_HEIGHT / (float)totalHeight;
    float viewportH = (float)gameHeight / (float)totalHeight;
    gameView.setViewport(sf::FloatRect({ 0.f, viewportY }, { 1.f, viewportH }));

    // Vue HUD : Elle regarde le score (0,0) et prend toute la largeur
    sf::View hudView = window.getDefaultView();

    // --- ASSETS & INITIALISATION ---
    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite mapSprite(tileset);

    sf::Texture pZ, pS, pQ, pD;
    pZ.loadFromFile("../Assets/Player/playerZ.png");
    pS.loadFromFile("../Assets/Player/playerS.png");
    pQ.loadFromFile("../Assets/Player/playerQ.png");
    pD.loadFromFile("../Assets/Player/playerD.png");
    PlayerTextures playerAssets = { pZ, pS, pQ, pD };

    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);
    openMap(map, collisions, "niveau2.txt");

    Player player(100.f, 300.f, playerAssets);
    TrailSystem trailSystem(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 6.f);

    ScoreSystem scoreSystem;
    if (!scoreSystem.init("../Assets/font.ttf")) {
        std::cout << "Erreur police" << std::endl;
    }

    // Fond pour le HUD (bande grise foncée en haut)
    sf::RectangleShape hudBackground({ (float)totalWidth, (float)HUD_HEIGHT });
    hudBackground.setFillColor(sf::Color(30, 30, 30)); // Gris très sombre

    sf::Clock clock;
    bool showHitbox = false;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) window.close();
                if (key->scancode == sf::Keyboard::Scancode::K) scoreSystem.addKill();

                // Touche F1 pour afficher/cacher la hitbox
                if (key->scancode == sf::Keyboard::Scancode::F1) showHitbox = !showHitbox;
            }
        }

        player.handleInput();
        player.update(deltaTime);
        scoreSystem.update(deltaTime);

        sf::FloatRect playerBounds = CollisionManager::getHitbox(player.getPosition(), 20.f);
        bool isDead = false;

        if (CollisionManager::checkMapCollision(playerBounds, collisions, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE)) isDead = true;
        else if (trailSystem.checkCollision(playerBounds, player.getVelocity())) isDead = true;

        if (isDead) {
            player.reset(100.f, 300.f);
            trailSystem.reset();
            scoreSystem.reset();
            continue;
        }

        trailSystem.addTrail(player.getPosition(), sf::Color::White);

        window.clear(sf::Color::Black);

        // --- PHASE 1 : DESSIN DU HUD (SCORE) ---
        // On remet la vue par défaut (tout l'écran, origine en haut à gauche)
        window.setView(hudView);
        window.draw(hudBackground); // Fond du score
        scoreSystem.draw(window);   // Le texte du score

        // --- PHASE 2 : DESSIN DU JEU ---
        // On active la vue "Jeu" (décalée vers le bas grâce au Viewport)
        window.setView(gameView);

        // Map
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

        trailSystem.draw(window);
        player.draw(window);

        // Utilisation de notre nouvelle fonction dans CollisionManager
        if (showHitbox) {
            CollisionManager::drawHitbox(window, playerBounds, sf::Color::Red);
        }

        window.display();
    }
    return 0;
}