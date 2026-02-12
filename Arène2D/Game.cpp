#include "Game.h"
#include "smallthing.cpp" 

Game::Game()
    : ticks(0.f),
    showHitbox(false),
    hasGameStarted(false),
    currentState(MENU),
    totalWidth(MAP_WIDTH* TILE_SIZE),
    gameHeight(MAP_HEIGHT* TILE_SIZE),
    totalHeight(gameHeight + HUD_HEIGHT),
    window(sf::VideoMode({ totalWidth, totalHeight }), "TRON GAME"),
    tileset(createTronTexture(TILE_SIZE, TEXTURE_COLS)),
    mapSprite(tileset)
{
    window.setFramerateLimit(60);

    gameView = sf::View(sf::FloatRect({ 0.f, 0.f }, { (float)totalWidth, (float)gameHeight }));

    float viewportY = (float)HUD_HEIGHT / (float)totalHeight;
    float viewportH = (float)gameHeight / (float)totalHeight;
    gameView.setViewport(sf::FloatRect({ 0.f, viewportY }, { 1.f, viewportH }));

    hudView = window.getDefaultView();

    hudBackground.setSize({ (float)totalWidth, (float)HUD_HEIGHT });
    hudBackground.setFillColor(sf::Color(30, 30, 30));

    if (!pZ.loadFromFile("../Assets/Player/playerZ.png") ||
        !pS.loadFromFile("../Assets/Player/playerS.png") ||
        !pQ.loadFromFile("../Assets/Player/playerQ.png") ||
        !pD.loadFromFile("../Assets/Player/playerD.png")) {
        std::cout << "Erreur chargement textures joueur" << std::endl;
    }
    PlayerTextures playerAssets = { pZ, pS, pQ, pD };

    if (!e1Z.loadFromFile("../Assets/Npc/Npc1Z.png") ||
        !e1S.loadFromFile("../Assets/Npc/Npc1S.png") ||
        !e1Q.loadFromFile("../Assets/Npc/Npc1Q.png") ||
        !e1D.loadFromFile("../Assets/Npc/Npc1D.png")) {
        std::cout << "Erreur chargement textures NPC 1" << std::endl;
    }
    PlayerTextures npc1Assets = { e1Z, e1S, e1Q, e1D };

    if (!e2Z.loadFromFile("../Assets/Npc/Npc2Z.png") ||
        !e2S.loadFromFile("../Assets/Npc/Npc2S.png") ||
        !e2Q.loadFromFile("../Assets/Npc/Npc2Q.png") ||
        !e2D.loadFromFile("../Assets/Npc/Npc2D.png")) {
        std::cout << "Erreur chargement textures NPC 2" << std::endl;
    }
    PlayerTextures npc2Assets = { e2Z, e2S, e2Q, e2D };

    map.resize(MAP_WIDTH * MAP_HEIGHT, 0);
    collisions.resize(MAP_WIDTH * MAP_HEIGHT, 0);

    player = new Player(100.f, 300.f, playerAssets);

    npc = new Npc(100.f, 300.f, npc1Assets);
    npc->Init();

    npc2 = new Npc(100.f, 300.f, npc2Assets);
    npc2->Init();

    trailSystem = new TrailSystem(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 6.f);
    trailSystem2 = new TrailSystem(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 6.f);
    trailSystem3 = new TrailSystem(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, 6.f);

    if (!scoreSystem.init("../Assets/Fonts/arial.ttf")) std::cout << "Erreur police score" << std::endl;

    mainMenu = new MainMenu((float)totalWidth, (float)totalHeight);
    if (!mainMenu->init("../Assets/Fonts/arial.ttf")) std::cout << "Erreur police menu" << std::endl;

    pauseMenu = new PauseMenu((float)totalWidth, (float)totalHeight);
    if (!pauseMenu->init("../Assets/Fonts/arial.ttf")) std::cout << "Erreur police pause" << std::endl;

    gameOverMenu = new GameOverMenu();
    if (!gameOverMenu->init("../Assets/Fonts/arial.ttf", (float)totalWidth, (float)totalHeight)) std::cout << "Erreur police gameover" << std::endl;

    if (!audioSystem.init()) std::cout << "Erreur init audio" << std::endl;
    audioSystem.playMenuMusic();
}

Game::~Game() {
    delete player;
    delete npc;
    delete npc2;
    delete trailSystem;
    delete trailSystem2;
    delete trailSystem3;
    delete mainMenu;
    delete pauseMenu;
    delete gameOverMenu;
}

void Game::openMap(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    int val; std::string tempStr; size_t count = 0;
    while (count < map.size() && file >> val) { map[count++] = val; }
    file.clear(); file >> tempStr; count = 0;
    while (count < collisions.size() && file >> val) { collisions[count++] = val; }
}

void Game::resetLevel() {
    hasGameStarted = false;
    player->reset(64.f, 448.f);
    trailSystem->reset();

    npc->reset(1536.f, 64.f);
    trailSystem2->reset();
    npc2->reset(1536.f, 834.f);
    trailSystem3->reset();
    scoreSystem.reset();

    audioSystem.playGameMusic();
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        float musicVol = mainMenu->m_optionsMenu.getMusicVolume() * 100.f;
        float sfxVol = mainMenu->m_optionsMenu.getSfxVolume() * 100.f;
        audioSystem.setGlobalVolume(musicVol, sfxVol);

        audioSystem.update();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();

        if (currentState == MENU) {
            int action = mainMenu->handleInput(window, *event);
            if (action == 1) {
                std::string levelFile = mainMenu->getSelectedLevelFile();
                openMap(levelFile);
                currentState = GAME;
                resetLevel();
            }
            if (action == 3) window.close();
        }
        else if (currentState == GAME) {
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) {
                    currentState = PAUSE;
                    audioSystem.playPauseMusic();
                }
                if (key->scancode == sf::Keyboard::Scancode::K) scoreSystem.addKill();
                if (key->scancode == sf::Keyboard::Scancode::F1) showHitbox = !showHitbox;
            }
        }
        else if (currentState == PAUSE) {
            int action = pauseMenu->handleInput(window, *event);
            if (action == 1) {
                currentState = GAME;
                audioSystem.playGameMusic();
            }
            if (action == 2) {
                currentState = MENU;
                hasGameStarted = false;
                audioSystem.playMenuMusic();
            }
        }
        else if (currentState == GAME_OVER) {
            GameOverAction action = gameOverMenu->handleEvent(*event, window);
            if (action == GameOverAction::Restart) {
                resetLevel();
                hasGameStarted = false;
                currentState = GAME;
                audioSystem.playGameMusic();
            }
            else if (action == GameOverAction::BackToMenu) {
                currentState = MENU;
                hasGameStarted = false;
                audioSystem.playMenuMusic();
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState == MENU) {
        mainMenu->update(deltaTime);
    }
    else if (currentState == GAME) {
        player->handleInput();
        player->update(deltaTime);
        npc->update(deltaTime, collisions, *trailSystem, *trailSystem2, *trailSystem3);
        npc2->update(deltaTime, collisions, *trailSystem, *trailSystem2, *trailSystem3);
        scoreSystem.update(deltaTime);

        if (!hasGameStarted) {
            if (player->getVelocity().x != 0 || player->getVelocity().y != 0) {
                hasGameStarted = true;
            }
        }

        if (hasGameStarted) {
            scoreSystem.update(deltaTime);
            npc->setActie(true);
            npc2->setActie(true);
        }

        sf::FloatRect playerBounds = CollisionManager::getHitbox(player->getPosition(), 18.f);
        sf::FloatRect npcBounds = CollisionManager::getHitbox(npc->getPosition(), 18.f);
        sf::FloatRect npc2Bounds = CollisionManager::getHitbox(npc2->getPosition(), 18.f);

        bool isDead = false;

        if (CollisionManager::checkMapCollision(playerBounds, collisions, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE)) isDead = true;
        else if (trailSystem->checkCollision(playerBounds, player->getVelocity())) isDead = true;
        else if (trailSystem2->checkCollision(playerBounds, player->getVelocity())) isDead = true;
        else if (trailSystem3->checkCollision(playerBounds, player->getVelocity())) isDead = true;

        if (isDead) {
            currentState = GAME_OVER;
            gameOverMenu->setState(false, scoreSystem.getScore());
            audioSystem.playGameOverMusic();
            audioSystem.playSound("DEATH_PLAYER");
        }

        bool iaDead = false;

        if (CollisionManager::checkMapCollision(npcBounds, collisions, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE)) iaDead = true;
        else if (trailSystem->checkCollision(npcBounds, npc->getVelocity())) iaDead = true;
        else if (trailSystem2->checkCollision(npcBounds, npc->getVelocity())) iaDead = true;
        else if (trailSystem3->checkCollision(npcBounds, npc->getVelocity())) iaDead = true;

        if (iaDead) {
            npc->reset(1536.f, 64.f);
            trailSystem2->reset();
            scoreSystem.addKill();
            audioSystem.playSound("DEATH_NPC");
        }
        iaDead = false;

        if (CollisionManager::checkMapCollision(npc2Bounds, collisions, MAP_WIDTH, MAP_HEIGHT, TILE_SIZE)) iaDead = true;
        else if (trailSystem->checkCollision(npc2Bounds, npc2->getVelocity())) iaDead = true;
        else if (trailSystem2->checkCollision(npc2Bounds, npc2->getVelocity())) iaDead = true;
        else if (trailSystem3->checkCollision(npc2Bounds, npc2->getVelocity())) iaDead = true;

        if (iaDead) {
            npc2->reset(1536.f, 834.f);
            trailSystem3->reset();
            scoreSystem.addKill();
            audioSystem.playSound("DEATH_NPC");
        }

        sf::Color playercol = sf::Color::White;
        if (mainMenu->m_optionsMenu.getrainbow()) {
            playercol = rainbowing(ticks);
        }
        trailSystem->addTrail(player->getPosition(), playercol);
        trailSystem2->addTrail(npc->getPosition(), sf::Color::Yellow);
        trailSystem3->addTrail(npc2->getPosition(), sf::Color::Magenta);

        if (mainMenu->m_optionsMenu.getspeedy()) {
            player->setSpeed(600.f);
            npc->setSpeed(600.f);
            npc2->setSpeed(600.f);
        }
        else if (mainMenu->m_optionsMenu.getslow()) {
            player->setSpeed(150.f);
            npc->setSpeed(150.f);
            npc2->setSpeed(150.f);
        }
        else {
            player->setSpeed(300.f);
            npc->setSpeed(300.f);
            npc2->setSpeed(300.f);
        }
        ticks += 100.f * deltaTime;
    }
    else if (currentState == PAUSE) {
        pauseMenu->update();
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    if (currentState == MENU) {
        window.setView(window.getDefaultView());
        mainMenu->draw(window);
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
        trailSystem->draw(window);
        player->draw(window);
        trailSystem2->draw(window);
        npc->draw(window);
        trailSystem3->draw(window);
        npc2->draw(window);

        if (currentState == GAME && showHitbox) {
            sf::FloatRect playerBounds = CollisionManager::getHitbox(player->getPosition(), 18.f);
            CollisionManager::drawHitbox(window, playerBounds, sf::Color::Red);

            sf::FloatRect npcBounds = CollisionManager::getHitbox(npc->getPosition(), 18.f);
            CollisionManager::drawHitbox(window, npcBounds, sf::Color::Red);

            sf::FloatRect npc2Bounds = CollisionManager::getHitbox(npc2->getPosition(), 18.f);
            CollisionManager::drawHitbox(window, npc2Bounds, sf::Color::Red);
        }

        if (currentState == PAUSE) {
            window.setView(window.getDefaultView());
            pauseMenu->draw(window);
        }

        if (currentState == GAME_OVER) {
            window.setView(window.getDefaultView());
            gameOverMenu->draw(window);
        }
    }

    window.display();
}