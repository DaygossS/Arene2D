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
#include "PauseMenu.hpp"
#include "GameOverMenu.hpp" 

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 5;
const int HUD_HEIGHT = 60;

enum GameState { MENU, GAME, PAUSE, OPTIONS, GAME_OVER };

static void openMap(std::vector<int>& mapData, std::vector<int>& colData, const std::string& filename) {
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

    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { (float)totalWidth, (float)gameHeight }));
    float viewportY = (float)HUD_HEIGHT / (float)totalHeight;
    float viewportH = (float)gameHeight / (float)totalHeight;
    gameView.setViewport(sf::FloatRect({ 0.f, viewportY }, { 1.f, viewportH }));

    sf::View hudView = window.getDefaultView();

    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite mapSprite(tileset);

    sf::Texture pZ, pS, pQ, pD;
    if (!pZ.loadFromFile("../Assets/Player/playerZ.png") ||
        !pS.loadFromFile("../Assets/Player/playerS.png") ||
        !pQ.loadFromFile("../Assets/Player/playerQ.png") ||
        !pD.loadFromFile("../Assets/Player/playerD.png")) {
        std::cout << "Erreur chargement textures joueur" << std::endl;
        return -1;
    }
    PlayerTextures playerAssets = { pZ, pS, pQ, pD };

    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);

    Player player(100.f, 300.f, playerAssets);
    TrailSystem trailSystem(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 6.f);

    ScoreSystem scoreSystem;
    if (!scoreSystem.init("../Assets/Fonts/arial.ttf")) std::cout << "Erreur police score" << std::endl;

    MainMenu mainMenu((float)totalWidth, (float)totalHeight);
    if (!mainMenu.init("../Assets/Fonts/arial.ttf")) std::cout << "Erreur police menu" << std::endl;

    PauseMenu pauseMenu((float)totalWidth, (float)totalHeight);
    if (!pauseMenu.init("../Assets/Fonts/arial.ttf")) std::cout << "Erreur police pause" << std::endl;

    GameOverMenu gameOverMenu;
    if (!gameOverMenu.init("../Assets/Fonts/arial.ttf", (float)totalWidth, (float)totalHeight)) std::cout << "Erreur police gameover" << std::endl;

    sf::RectangleShape hudBackground({ (float)totalWidth, (float)HUD_HEIGHT });
    hudBackground.setFillColor(sf::Color(30, 30, 30));

    sf::Clock clock;
    bool showHitbox = false;
    bool hasGameStarted = false;

    GameState currentState = MENU;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (currentState == MENU) {
                int action = mainMenu.handleInput(window, *event);
                if (action == 1) {
                    std::string levelFile = mainMenu.getSelectedLevelFile();
                    openMap(map, collisions, levelFile);
                    currentState = GAME;
                    hasGameStarted = false;
                    player.reset(100.f, 300.f);
                    trailSystem.reset();
                    scoreSystem.reset();
                }
                if (action == 3) window.close();
            }
            else if (currentState == GAME) {
                if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->scancode == sf::Keyboard::Scancode::Escape) currentState = PAUSE;
                    if (key->scancode == sf::Keyboard::Scancode::K) scoreSystem.addKill();
                    if (key->scancode == sf::Keyboard::Scancode::F1) showHitbox = !showHitbox;
                }
            }
            else if (currentState == PAUSE) {
                int action = pauseMenu.handleInput(window, *event);
                if (action == 1) currentState = GAME;
                if (action == 2) {
                    currentState = MENU;
                    hasGameStarted = false;
                }
            }
            else if (currentState == GAME_OVER) {
                GameOverAction action = gameOverMenu.handleEvent(*event, window);
                if (action == GameOverAction::Restart) {
                    player.reset(100.f, 300.f);
                    trailSystem.reset();
                    scoreSystem.reset();
                    hasGameStarted = false;
                    currentState = GAME;
                }
                else if (action == GameOverAction::BackToMenu) {
                    currentState = MENU;
                    hasGameStarted = false;
                }
            }
        }

        if (currentState == MENU) {
            mainMenu.update(deltaTime);
        }
        else if (currentState == GAME) {
            player.handleInput();
            player.update(deltaTime);

            if (!hasGameStarted) {
                if (player.getVelocity().x != 0 || player.getVelocity().y != 0) {
                    hasGameStarted = true;
                }
            }

            if (hasGameStarted) {
                scoreSystem.update(deltaTime);
            }

            sf::FloatRect playerBounds = CollisionManager::getHitbox(player.getPosition(), 20.f);
            bool isDead = false;

            if (CollisionManager::checkMapCollision(playerBounds, collisions, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE)) isDead = true;
            else if (trailSystem.checkCollision(playerBounds, player.getVelocity())) isDead = true;

            if (isDead) {
                currentState = GAME_OVER;
                gameOverMenu.setState(false, scoreSystem.getScore());
            }

            trailSystem.addTrail(player.getPosition(), sf::Color::White);
        }
        else if (currentState == PAUSE) {
            pauseMenu.update();
        }

        window.clear(sf::Color::Black);

        if (currentState == MENU) {
            window.setView(window.getDefaultView());
            mainMenu.draw(window);
        }
        else if (currentState == GAME || currentState == PAUSE || currentState == GAME_OVER) {
            window.setView(hudView);
            window.draw(hudBackground);
            scoreSystem.draw(window);

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

            if (currentState == GAME && showHitbox) {
                sf::FloatRect currentBounds = CollisionManager::getHitbox(player.getPosition(), 20.f);
                CollisionManager::drawHitbox(window, currentBounds, sf::Color::Red);
            }

            if (currentState == PAUSE) {
                window.setView(window.getDefaultView());
                pauseMenu.draw(window);
            }

            if (currentState == GAME_OVER) {
                window.setView(window.getDefaultView());
                gameOverMenu.draw(window);
            }
        }

        window.display();
    }
    return 0;
}