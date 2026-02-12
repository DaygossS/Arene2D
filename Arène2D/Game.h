#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

// Tes includes
#include "WorldAssets.hpp"
#include "Player.hpp"
#include "TrailSystem.hpp"
#include "CollisionManager.hpp"
#include "ScoreSystem.hpp"
#include "MainMenu.hpp"
#include "OptionsMenu.hpp"
#include "PauseMenu.hpp"
#include "GameOverMenu.hpp" 
#include "AudioSystem.hpp"
#include "Bike.h" // Npc

enum GameState { MENU, GAME, PAUSE, OPTIONS, GAME_OVER };

class Game {
private:
    // --- Constantes ---
    const int TILE_SIZE = 32;
    const int MAP_WIDTH = 50;
    const int MAP_HEIGHT = 28;
    const int TEXTURE_COLS = 5;
    const int HUD_HEIGHT = 60;

    // --- Variables de la Fenêtre ---
    unsigned int totalWidth;
    unsigned int gameHeight;
    unsigned int totalHeight;
    sf::RenderWindow window;
    sf::View gameView;
    sf::View hudView;
    sf::Clock clock;
    float ticks;

    // --- Assets Graphiques ---
    // IMPORTANT : tileset doit être déclaré AVANT mapSprite pour l'initialisation
    sf::Texture tileset;
    sf::Sprite mapSprite;
    sf::RectangleShape hudBackground;

    // Textures Player
    sf::Texture pZ, pS, pQ, pD;
    // Textures NPC 1
    sf::Texture e1Z, e1S, e1Q, e1D;
    // Textures NPC 2
    sf::Texture e2Z, e2S, e2Q, e2D;

    // --- Données de la Map ---
    std::vector<int> map;
    std::vector<int> collisions;

    // --- Entités et Systèmes ---
    Player* player;
    Npc* npc;
    Npc* npc2;

    TrailSystem* trailSystem;
    TrailSystem* trailSystem2;
    TrailSystem* trailSystem3;

    ScoreSystem scoreSystem;
    AudioSystem audioSystem;

    // --- Menus ---
    MainMenu* mainMenu;
    PauseMenu* pauseMenu;
    GameOverMenu* gameOverMenu;

    // --- États du Jeu ---
    GameState currentState;
    bool showHitbox;
    bool hasGameStarted;

    // --- Méthodes Privées ---
    void openMap(const std::string& filename);
    void processEvents();
    void update(float deltaTime);
    void render();
    void resetLevel();

public:
    Game();
    ~Game();
    void run();
};