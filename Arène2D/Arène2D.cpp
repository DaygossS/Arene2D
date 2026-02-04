#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include "WorldAssets.hpp"

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int SIDEBAR_WIDTH = 200;
const int TEXTURE_COLS = 4;

void saveMap(const std::vector<int>& mapData, const std::vector<int>& colData, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < mapData.size(); ++i) {
            file << mapData[i] << " ";
            if ((i + 1) % MAP_WIDTH == 0) file << "\n";
        }
        file << "\n---COLLISIONS---\n";
        for (int i = 0; i < colData.size(); ++i) {
            file << colData[i] << " ";
            if ((i + 1) % MAP_WIDTH == 0) file << "\n";
        }
        std::cout << "Saved" << std::endl;
    }
}

void openMap(std::vector<int>& mapData, std::vector<int>& colData, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    int val;
    std::string tempStr;
    int count = 0;
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
    std::cout << "Loaded" << std::endl;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE + SIDEBAR_WIDTH), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "TRON EDITOR");
    window.setFramerateLimit(60);

    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite sprite(tileset);

    sf::RectangleShape collisionRect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    collisionRect.setFillColor(sf::Color(255, 0, 0, 60));
    collisionRect.setOutlineColor(sf::Color::Red);
    collisionRect.setOutlineThickness(1);

    sf::RectangleShape sidebarBg(sf::Vector2f(SIDEBAR_WIDTH, MAP_HEIGHT * TILE_SIZE));
    sidebarBg.setPosition({ (float)(MAP_WIDTH * TILE_SIZE), 0.f });
    sidebarBg.setFillColor(sf::Color(20, 20, 20));
    sidebarBg.setOutlineColor(sf::Color::Cyan);
    sidebarBg.setOutlineThickness(-2);

    sf::RectangleShape selectionRect(sf::Vector2f(TILE_SIZE + 4, TILE_SIZE + 4));
    selectionRect.setFillColor(sf::Color::Transparent);
    selectionRect.setOutlineColor(sf::Color::White);
    selectionRect.setOutlineThickness(2);

    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);

    int currentTileID = 1;
    bool editCollisionMode = false;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::S) saveMap(map, collisions, "niveau4.txt");
                if (key->scancode == sf::Keyboard::Scancode::O) openMap(map, collisions, "niveau3.txt");
                if (key->scancode == sf::Keyboard::Scancode::C) {
                    editCollisionMode = !editCollisionMode;
                    if (editCollisionMode) window.setTitle("MODE COLLISION");
                    else window.setTitle("MODE DECOR");
                }
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (mousePos.x < MAP_WIDTH * TILE_SIZE && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.y < MAP_HEIGHT * TILE_SIZE) {
                int gridX = mousePos.x / TILE_SIZE;
                int gridY = mousePos.y / TILE_SIZE;
                int index = gridY * MAP_WIDTH + gridX;

                if (editCollisionMode) collisions[index] = 1;
                else map[index] = currentTileID;
            }
            else if (mousePos.x >= MAP_WIDTH * TILE_SIZE) {
                int relativeX = mousePos.x - (MAP_WIDTH * TILE_SIZE + 20);
                int relativeY = mousePos.y - 50;

                if (relativeX >= 0 && relativeY >= 0) {
                    int col = relativeX / (TILE_SIZE + 10);
                    int row = relativeY / (TILE_SIZE + 10);

                    if (col < 2) {
                        int clickedID = row * 2 + col;
                        if (clickedID < TEXTURE_COLS) {
                            currentTileID = clickedID;
                            if (editCollisionMode) editCollisionMode = false;
                        }
                    }
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            if (mousePos.x < MAP_WIDTH * TILE_SIZE && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.y < MAP_HEIGHT * TILE_SIZE) {
                int gridX = mousePos.x / TILE_SIZE;
                int gridY = mousePos.y / TILE_SIZE;
                int index = gridY * MAP_WIDTH + gridX;
                if (editCollisionMode) collisions[index] = 0;
            }
        }

        window.clear(sf::Color::Black);

        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int index = y * MAP_WIDTH + x;

                int tID = map[index];
                if (tID >= TEXTURE_COLS) tID = 0;
                sprite.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });
                sprite.setTextureRect(sf::IntRect({ tID * TILE_SIZE, 0 }, { TILE_SIZE, TILE_SIZE }));

                if (editCollisionMode) sprite.setColor(sf::Color(100, 100, 100));
                else sprite.setColor(sf::Color::White);

                window.draw(sprite);

                if (editCollisionMode && collisions[index] == 1) {
                    collisionRect.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });
                    window.draw(collisionRect);
                }
            }
        }

        window.draw(sidebarBg);

        int paletteStartX = MAP_WIDTH * TILE_SIZE + 20;
        int paletteStartY = 50;

        for (int i = 0; i < TEXTURE_COLS; i++) {
            int pX = paletteStartX + (i % 2) * (TILE_SIZE + 10);
            int pY = paletteStartY + (i / 2) * (TILE_SIZE + 10);

            sprite.setPosition({ (float)pX, (float)pY });
            sprite.setTextureRect(sf::IntRect({ i * TILE_SIZE, 0 }, { TILE_SIZE, TILE_SIZE }));
            sprite.setColor(sf::Color::White);
            window.draw(sprite);

            if (currentTileID == i) {
                selectionRect.setPosition({ (float)pX - 2, (float)pY - 2 });
                window.draw(selectionRect);
            }
        }

        if (mousePos.x < MAP_WIDTH * TILE_SIZE && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.y < MAP_HEIGHT * TILE_SIZE) {
            int gridX = mousePos.x / TILE_SIZE;
            int gridY = mousePos.y / TILE_SIZE;

            if (editCollisionMode) {
                sf::RectangleShape cursor = collisionRect;
                cursor.setPosition({ (float)gridX * TILE_SIZE, (float)gridY * TILE_SIZE });
                cursor.setFillColor(sf::Color(255, 0, 0, 150));
                window.draw(cursor);
            }
            else {
                sf::Sprite cursor = sprite;
                cursor.setPosition({ (float)gridX * TILE_SIZE, (float)gridY * TILE_SIZE });
                cursor.setTextureRect(sf::IntRect({ currentTileID * TILE_SIZE, 0 }, { TILE_SIZE, TILE_SIZE }));
                cursor.setColor(sf::Color(255, 255, 255, 150));
                window.draw(cursor);
            }
        }

        window.display();
    }
    return 0;
}