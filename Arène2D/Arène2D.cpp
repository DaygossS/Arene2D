#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 4;

sf::Texture createTestTexture() {
    sf::Image img;
    img.resize({ static_cast<unsigned int>(TILE_SIZE * TEXTURE_COLS), static_cast<unsigned int>(TILE_SIZE) });

    for (int x = 0; x < TILE_SIZE; x++) for (int y = 0; y < TILE_SIZE; y++) img.setPixel({ (unsigned)x, (unsigned)y }, sf::Color::Black);
    for (int x = 32; x < 64; x++) for (int y = 0; y < TILE_SIZE; y++) img.setPixel({ (unsigned)x, (unsigned)y }, sf::Color::Green);
    for (int x = 64; x < 96; x++) for (int y = 0; y < TILE_SIZE; y++) img.setPixel({ (unsigned)x, (unsigned)y }, sf::Color(100, 100, 100));
    for (int x = 96; x < 128; x++) for (int y = 0; y < TILE_SIZE; y++) img.setPixel({ (unsigned)x, (unsigned)y }, sf::Color(139, 69, 19));

    sf::Texture tex;
    tex.loadFromImage(img);
    return tex;
}

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
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "Editeur");
    window.setFramerateLimit(60);

    sf::Texture tileset = createTestTexture();
    sf::Sprite sprite(tileset);

    sf::RectangleShape collisionRect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    collisionRect.setFillColor(sf::Color(255, 0, 0, 100));
    collisionRect.setOutlineColor(sf::Color::Red);
    collisionRect.setOutlineThickness(1);

    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);

    int currentTileID = 1;
    bool editCollisionMode = false;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            else if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (!editCollisionMode) {
                    if (wheel->delta > 0) currentTileID++; else currentTileID--;
                    if (currentTileID >= TEXTURE_COLS) currentTileID = 0;
                    if (currentTileID < 0) currentTileID = TEXTURE_COLS - 1;
                }
            }

            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::S) saveMap(map, collisions, "niveau.txt");
                if (key->scancode == sf::Keyboard::Scancode::O) openMap(map, collisions, "niveau.txt");

                if (key->scancode == sf::Keyboard::Scancode::C) {
                    editCollisionMode = !editCollisionMode;
                    if (editCollisionMode) window.setTitle("MODE COLLISION");
                    else window.setTitle("MODE DECOR");
                }
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int gridX = mousePos.x / TILE_SIZE;
        int gridY = mousePos.y / TILE_SIZE;
        bool inside = gridX >= 0 && gridX < MAP_WIDTH && gridY >= 0 && gridY < MAP_HEIGHT;

        if (inside) {
            int index = gridY * MAP_WIDTH + gridX;
            if (editCollisionMode) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) collisions[index] = 1;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) collisions[index] = 0;
            }
            else {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) map[index] = currentTileID;
            }
        }

        window.clear(sf::Color(50, 50, 50));

        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int index = y * MAP_WIDTH + x;

                int tID = map[index];
                if (tID >= TEXTURE_COLS) tID = 0;
                sprite.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });
                sprite.setTextureRect(sf::IntRect({ tID * TILE_SIZE, 0 }, { TILE_SIZE, TILE_SIZE }));
                window.draw(sprite);

                if (editCollisionMode && collisions[index] == 1) {
                    collisionRect.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });
                    window.draw(collisionRect);
                }
            }
        }

        if (inside) {
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