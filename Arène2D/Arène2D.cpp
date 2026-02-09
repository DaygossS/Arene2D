#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include "WorldAssets.hpp"
#include "Player.hpp"
#include "TrailSystem.hpp"
#include "CollisionManager.hpp"
#include "ScoreSystem.hpp"
#include "MainMenu.hpp" 
#include "OptionsMenu.hpp"


const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 5;
const int HUD_HEIGHT = 60;
#include "Bike.cpp"
// Énumération pour gérer l'état du jeu
enum GameState { MENU, GAME, OPTIONS };

void openMap(std::vector<int>& mapData, std::vector<int>& colData, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    int val; std::string tempStr; size_t count = 0;
    while (count < mapData.size() && file >> val) { mapData[count++] = val; }
    file.clear(); file >> tempStr; count = 0;
    while (count < colData.size() && file >> val) { colData[count++] = val; }
}

int main() {
    unsigned int totalWidth = MAP_WIDTH * TILE_SIZE;
    unsigned int gameHeight = MAP_HEIGHT * TILE_SIZE;
    unsigned int totalHeight = gameHeight + HUD_HEIGHT;

    sf::RenderWindow window(sf::VideoMode({ totalWidth, totalHeight }), "TRON GAME");
    window.setFramerateLimit(60);

    // --- VUES ---
    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { (float)totalWidth, (float)gameHeight }));
    float viewportY = (float)HUD_HEIGHT / (float)totalHeight;
    float viewportH = (float)gameHeight / (float)totalHeight;
    gameView.setViewport(sf::FloatRect({ 0.f, viewportY }, { 1.f, viewportH }));

    sf::View hudView = window.getDefaultView();

    // --- ASSETS ---
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

    // --- OBJETS JEU ---
    Player player(100.f, 300.f, playerAssets);
    Npc npc(1000.f, 500.f, playerAssets);
    TrailSystem trailSystem(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 6.f);
    TrailSystem trailSystem2(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 6.f);

    ScoreSystem scoreSystem;
    if (!scoreSystem.init("../Assets/font.ttf")) std::cout << "Erreur police score" << std::endl;

    // --- MENU ---
    MainMenu mainMenu((float)totalWidth, (float)totalHeight);
    if (!mainMenu.init("../Assets/font.ttf")) std::cout << "Erreur police menu" << std::endl;

    sf::RectangleShape hudBackground({ (float)totalWidth, (float)HUD_HEIGHT });
    hudBackground.setFillColor(sf::Color(30, 30, 30));

    sf::Clock clock;
    bool showHitbox = false;

    // --- ÉTAT INITIAL ---
    GameState currentState = MENU;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // 1. GESTION DES ÉVÉNEMENTS
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            // Si on est dans le MENU
            if (currentState == MENU) {
                int action = mainMenu.handleInput(window, *event);
                if (action == 1) currentState = GAME;       // Bouton JOUER
                if (action == 2) std::cout << "Options..." << std::endl; // Bouton OPTIONS (Vide pour l'instant)
                if (action == 3) window.close();            // Bouton QUITTER
            }

            // Si on est en JEU
            else if (currentState == GAME) {
                if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scancode::Escape) currentState = MENU; // Echap retourne au menu
                    if (key->scancode == sf::Keyboard::Scancode::K) scoreSystem.addKill();
                    if (key->scancode == sf::Keyboard::Scancode::F1) showHitbox = !showHitbox;
                }
            }
        }

        window.clear(sf::Color::Black);

        // 2. LOGIQUE & AFFICHAGE SELON L'ÉTAT
        if (currentState == MENU) {
            // Pour le menu, on utilise la vue par défaut (tout l'écran)
            window.setView(window.getDefaultView());
            mainMenu.draw(window);
        }
        else if (currentState == GAME) {
            // --- LOGIQUE DU JEU ---
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
                npc.reset(1000.f, 500.f);
                trailSystem2.reset();
                scoreSystem.reset();
                // Optionnel : currentState = MENU; // Si tu veux retourner au menu quand tu meurs
            }

            trailSystem.addTrail(player.getPosition(), sf::Color::White);

            // --- AFFICHAGE DU JEU ---

            // A. HUD
            window.setView(hudView);
            window.draw(hudBackground);
            scoreSystem.draw(window);

            // B. MONDE
            window.setView(gameView);
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
            trailSystem2.draw(window);
            npc.draw(window);
            if (showHitbox) CollisionManager::drawHitbox(window, playerBounds, sf::Color::Red);
        }

        window.display();
    }
    return 0;
}