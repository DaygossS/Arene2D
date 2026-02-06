#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "WorldAssets.hpp"
#include "Player.hpp"
#include <iostream>

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 5;


#include "Bike.cpp"

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
        { bounds.position.x, bounds.position.y },
        { bounds.position.x + bounds.size.x, bounds.position.y },
        { bounds.position.x, bounds.position.y + bounds.size.y },
        { bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y }
    };

    for (int i = 0; i < 4; i++) {
        int gridX = (int)(points[i].x / TILE_SIZE);
        int gridY = (int)(points[i].y / TILE_SIZE);

        if (gridX >= 0 && gridX < MAP_WIDTH && gridY >= 0 && gridY < MAP_HEIGHT) {
            int index = gridY * MAP_WIDTH + gridX;
            if (collisions[index] == 1) return true;
        }
        else {
            return true; 
        }
    }
    return false;
}



bool isCollidingWithTrail(const sf::FloatRect& bounds, const sf::Vector2f& velocity, const sf::Image& mask) {

    float margin = 2.f;
    std::vector<sf::Vector2f> checkPoints;

    if (velocity.x > 0) {
        checkPoints.push_back({ bounds.position.x + bounds.size.x, bounds.position.y + margin });
        checkPoints.push_back({ bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y - margin });
    }
    else if (velocity.x < 0) {
        checkPoints.push_back({ bounds.position.x, bounds.position.y + margin });
        checkPoints.push_back({ bounds.position.x, bounds.position.y + bounds.size.y - margin });
    }
    else if (velocity.y > 0) {
        checkPoints.push_back({ bounds.position.x + margin, bounds.position.y + bounds.size.y });
        checkPoints.push_back({ bounds.position.x + bounds.size.x - margin, bounds.position.y + bounds.size.y });
    }
    else if (velocity.y < 0) {
        checkPoints.push_back({ bounds.position.x + margin, bounds.position.y });
        checkPoints.push_back({ bounds.position.x + bounds.size.x - margin, bounds.position.y });
    }

    sf::Vector2u maskSize = mask.getSize();
    for (const auto& point : checkPoints) {
        unsigned int x = (unsigned int)point.x;
        unsigned int y = (unsigned int)point.y;

        if (x < maskSize.x && y < maskSize.y) {
            if (mask.getPixel({ x, y }) == sf::Color::White || mask.getPixel({ x, y }) == sf::Color::Red) {
                return true;
            }
        }
    }
    return false;
}


int main() {





    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "TRON GAME");
    window.setFramerateLimit(60);

    
    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite mapSprite(tileset);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../Assets/player.png")) {
        std::cerr << "impossible de charger l'asset" << std::endl;
    }

    //Player Draw
    sf::RenderTexture trailTexture;
    trailTexture.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) });
    trailTexture.clear(sf::Color::Transparent);
    sf::Sprite trailDisplay(trailTexture.getTexture());
    sf::Image trailMask;
    trailMask.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);
    sf::RectangleShape trailBrush;
    trailBrush.setFillColor(sf::Color::White); 



    //IA? Draw
    sf::RenderTexture trailTexture2;
    trailTexture2.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) });
    trailTexture2.clear(sf::Color::Transparent);
    sf::Sprite trailDisplay2(trailTexture2.getTexture());
    sf::Image trailMask2;
    trailMask2.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);
    sf::RectangleShape trailBrush2;
    trailBrush2.setFillColor(sf::Color::Red);





    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);
    
    openMap(map, collisions, "niveau2.txt");

    
    Player player(100.f, 300.f, playerTexture);
    Npc npc(1000.f,500.f,playerTexture);
    


    npc.Init();


    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }
        }


        npc.Update(deltaTime,collisions,trailMask,trailMask2);


        player.handleInput();
        player.update(deltaTime);

        

        sf::FloatRect playerBounds(player.getPosition(), player.getScale());
        bool isDead = false;

        
        if (isCollidingWithMap(playerBounds, collisions)) {
            isDead = true;
        }
        else if (isCollidingWithTrail(playerBounds, player.getVelocity(), trailMask)) {
            isDead = true;
        }
        else if (isCollidingWithTrail(playerBounds, player.getVelocity(), trailMask2)) {
            isDead = true;
        }

        if (isDead) {

            player = Player(100.f, 300.f, playerTexture);
            //npc = Npc(1000.f, 500.f, playerTexture);


            //player Draw
            trailTexture.clear(sf::Color::Transparent);
            trailMask.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);

            trailTexture2.clear(sf::Color::Transparent);
            trailMask2.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);

            
            continue;
        }



        sf::FloatRect npcBounds(npc.GetPosition(), npc.getSize());
        bool IAisDead = false;
        if (isCollidingWithMap(npcBounds, collisions)) {
            IAisDead = true;
        }
        else if (isCollidingWithTrail(npcBounds, npc.getVelocity(), trailMask)) {
            IAisDead = true;
        }
        else if (isCollidingWithTrail(npcBounds, npc.getVelocity(), trailMask2)) {
            IAisDead = true;
        }

        if (IAisDead) {

            //player = Player(100.f, 300.f, playerTexture);
            //npc = Npc(1000.f, 500.f, playerTexture);


            //player Draw
            trailTexture.clear(sf::Color::Transparent);
            trailMask.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);

            trailTexture2.clear(sf::Color::Transparent);
            trailMask2.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);


            continue;
        }


        //player Draw
        float trailW = playerBounds.size.x - 26.f;
        float trailH = playerBounds.size.y - 26.f;
        if (trailW < 2.f) trailW = 2.f;
        if (trailH < 2.f) trailH = 2.f;

        trailBrush.setSize({ trailW, trailH });
        sf::Vector2f playerCenter = player.getPosition();
        float brushX = playerCenter.x - (trailW / 2.f);
        float brushY = playerCenter.y - (trailH / 2.f);
        trailBrush.setPosition({ brushX, brushY });

        trailTexture.draw(trailBrush);
        trailTexture.display();

        int startX = (int)brushX;
        int startY = (int)brushY;
        int endX = startX + (int)trailW;
        int endY = startY + (int)trailH;

        sf::Vector2u maskSize = trailMask.getSize();

        for (int x = startX; x < endX; x++) {
            for (int y = startY; y < endY; y++) {
                if (x >= 0 && x < (int)maskSize.x && y >= 0 && y < (int)maskSize.y) {
                    trailMask.setPixel({ (unsigned int)x, (unsigned int)y }, sf::Color::White);
                }
            }
        }

        //IA Draw
        trailW = npcBounds.size.x - 26.f;
        trailH = npcBounds.size.y - 26.f;
        if (trailW < 2.f) trailW = 2.f;
        if (trailH < 2.f) trailH = 2.f;

        trailBrush2.setSize({ trailW, trailH });
        sf::Vector2f npcCenter = npc.GetPosition();
        brushX = npcCenter.x - (trailW / 2.f);
        brushY = npcCenter.y - (trailH / 2.f);
        trailBrush2.setPosition({ brushX, brushY });

        trailTexture2.draw(trailBrush2);
        trailTexture2.display();

        startX = (int)brushX;
        startY = (int)brushY;
        endX = startX + (int)trailW;
        endY = startY + (int)trailH;

        sf::Vector2u maskSize2 = trailMask2.getSize();

        for (int x = startX; x < endX; x++) {
            for (int y = startY; y < endY; y++) {
                if (x >= 0 && x < (int)maskSize2.x && y >= 0 && y < (int)maskSize2.y) {
                    trailMask2.setPixel({ (unsigned int)x, (unsigned int)y }, sf::Color::Red);
                }
            }
        }





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

        //player draw
        window.draw(trailDisplay);
        window.draw(trailDisplay2);






        player.draw(window);
        npc.draw(window);

        window.display();
    }
    return 0;
}