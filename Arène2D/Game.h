#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

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
#include "Bike.h"

enum GameState { MENU, GAME, PAUSE, OPTIONS, GAME_OVER };

class Game {
private:
    const int TILE_SIZE = 32;
    const int MAP_WIDTH = 50;
    const int MAP_HEIGHT = 28;
    const int TEXTURE_COLS = 5;
    const int HUD_HEIGHT = 60;

    unsigned int totalWidth;
    unsigned int gameHeight;
    unsigned int totalHeight;
    sf::RenderWindow window;
    sf::View gameView;
    sf::View hudView;
    sf::Clock clock;
    float ticks;

    sf::Texture tileset;
    sf::Sprite mapSprite;
    sf::RectangleShape hudBackground;

    sf::Texture pZ, pS, pQ, pD;
    sf::Texture pSpecZ, pSpecS, pSpecQ, pSpecD;

    PlayerTextures playerAssetsDefault;
    PlayerTextures playerAssetsSpecial;

    sf::Texture e1Z, e1S, e1Q, e1D;
    sf::Texture e2Z, e2S, e2Q, e2D;

    std::vector<int> map;
    std::vector<int> collisions;

    Player* player;
    Npc* npc;
    Npc* npc2;

    TrailSystem* trailSystem;
    TrailSystem* trailSystem2;
    TrailSystem* trailSystem3;

    ScoreSystem scoreSystem;
    AudioSystem audioSystem;

    MainMenu* mainMenu;
    PauseMenu* pauseMenu;
    GameOverMenu* gameOverMenu;

    GameState currentState;
    bool showHitbox;
    bool hasGameStarted;

    bool isSpecialSkinActive;
    int currentLevelIndex;

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