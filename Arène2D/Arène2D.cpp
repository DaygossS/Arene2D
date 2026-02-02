#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

// --- Configuration ---
const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int SIDEBAR_WIDTH = 200; // Espace pour le menu à droite
const int TEXTURE_COLS = 4;    // Nombre de tuiles différentes dans ton image

// Fonction pour créer une texture de test SI tu n'as pas encore ton image
// Une fois que tu as ton image, tu n'auras plus besoin de cette fonction
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
    // La fenêtre est plus large pour accueillir le menu
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE + SIDEBAR_WIDTH), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "Editeur de Niveau");
    window.setFramerateLimit(60);

    // --- CHARGEMENT TEXTURE ---
    sf::Texture tileset;
    // Si tu as ton fichier, décommente la ligne ci-dessous :
     if (!tileset.loadFromFile("../Assets/tileset/9.png")) return -1;

    // Pour l'instant, on utilise la texture générée par le code :
    //tileset = createTestTexture();

    sf::Sprite sprite(tileset);

    sf::RectangleShape collisionRect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    collisionRect.setFillColor(sf::Color(255, 0, 0, 100));
    collisionRect.setOutlineColor(sf::Color::Red);
    collisionRect.setOutlineThickness(1);

    // Fond du menu de droite
    sf::RectangleShape sidebarBg(sf::Vector2f(SIDEBAR_WIDTH, MAP_HEIGHT * TILE_SIZE));
    sidebarBg.setPosition({ (float)(MAP_WIDTH * TILE_SIZE), 0.f });
    sidebarBg.setFillColor(sf::Color(30, 30, 30));

    // Cadre de sélection dans le menu
    sf::RectangleShape selectionRect(sf::Vector2f(TILE_SIZE + 4, TILE_SIZE + 4));
    selectionRect.setFillColor(sf::Color::Transparent);
    selectionRect.setOutlineColor(sf::Color::Yellow);
    selectionRect.setOutlineThickness(2);

    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);

    int currentTileID = 1;
    bool editCollisionMode = false;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::S) saveMap(map, collisions, "niveau.txt");
                if (key->scancode == sf::Keyboard::Scancode::O) openMap(map, collisions, "niveau.txt");
                if (key->scancode == sf::Keyboard::Scancode::C) {
                    editCollisionMode = !editCollisionMode;
                    if (editCollisionMode) window.setTitle("MODE COLLISION (Clic G: Bloquer / Clic D: Debloquer)");
                    else window.setTitle("MODE DECOR (Clic G: Peindre / Palette a droite)");
                }
            }
        }

        // --- GESTION SOURIS ---
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Clic Gauche
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            // Zone CARTE
            if (mousePos.x < MAP_WIDTH * TILE_SIZE && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.y < MAP_HEIGHT * TILE_SIZE) {
                int gridX = mousePos.x / TILE_SIZE;
                int gridY = mousePos.y / TILE_SIZE;
                int index = gridY * MAP_WIDTH + gridX;

                if (editCollisionMode) collisions[index] = 1;
                else map[index] = currentTileID;
            }
            // Zone SIDEBAR (Palette)
            else if (mousePos.x >= MAP_WIDTH * TILE_SIZE) {
                // On vérifie si on clique sur une tuile de la palette
                // Logique inverse de l'affichage plus bas
                int relativeX = mousePos.x - (MAP_WIDTH * TILE_SIZE + 20);
                int relativeY = mousePos.y - 50;

                // On check grossièrement si on est dans la zone d'une tuile
                if (relativeX >= 0 && relativeY >= 0) {
                    // On suppose 2 colonnes dans la palette
                    int col = relativeX / (TILE_SIZE + 10);
                    int row = relativeY / (TILE_SIZE + 10);

                    // Si on clique bien sur les colonnes 0 ou 1
                    if (col < 2) {
                        int clickedID = row * 2 + col;
                        if (clickedID < TEXTURE_COLS) {
                            currentTileID = clickedID;
                            // Si on sélectionne une tuile, on repasse en mode Décor automatiquement
                            if (editCollisionMode) {
                                editCollisionMode = false;
                                window.setTitle("MODE DECOR (Selection Auto)");
                            }
                        }
                    }
                }
            }
        }

        // Clic Droit (Gomme / Débloquer)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            if (mousePos.x < MAP_WIDTH * TILE_SIZE && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.y < MAP_HEIGHT * TILE_SIZE) {
                int gridX = mousePos.x / TILE_SIZE;
                int gridY = mousePos.y / TILE_SIZE;
                int index = gridY * MAP_WIDTH + gridX;
                if (editCollisionMode) collisions[index] = 0;
            }
        }

        window.clear(sf::Color(50, 50, 50));

        // 1. DESSINER LA CARTE
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int index = y * MAP_WIDTH + x;

                // Layer Decor
                int tID = map[index];
                if (tID >= TEXTURE_COLS) tID = 0;
                sprite.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });
                sprite.setTextureRect(sf::IntRect({ tID * TILE_SIZE, 0 }, { TILE_SIZE, TILE_SIZE }));
                // En mode collision, on assombrit le décor pour mieux voir le rouge
                if (editCollisionMode) sprite.setColor(sf::Color(150, 150, 150));
                else sprite.setColor(sf::Color::White);
                window.draw(sprite);

                // Layer Collision
                if (editCollisionMode && collisions[index] == 1) {
                    collisionRect.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });
                    window.draw(collisionRect);
                }
            }
        }

        // 2. DESSINER LA SIDEBAR (Palette)
        window.draw(sidebarBg);

        int paletteStartX = MAP_WIDTH * TILE_SIZE + 20;
        int paletteStartY = 50;

        for (int i = 0; i < TEXTURE_COLS; i++) {
            // Disposition en 2 colonnes
            int pX = paletteStartX + (i % 2) * (TILE_SIZE + 10);
            int pY = paletteStartY + (i / 2) * (TILE_SIZE + 10);

            sprite.setPosition({ (float)pX, (float)pY });
            sprite.setTextureRect(sf::IntRect({ i * TILE_SIZE, 0 }, { TILE_SIZE, TILE_SIZE }));
            sprite.setColor(sf::Color::White); // Toujours blanc dans la palette
            window.draw(sprite);

            // Cadre jaune autour de la sélection actuelle
            if (currentTileID == i) {
                selectionRect.setPosition({ (float)pX - 2, (float)pY - 2 });
                window.draw(selectionRect);
            }
        }

        // 3. CURSEUR SOURIS (Uniquement sur la carte)
        if (mousePos.x < MAP_WIDTH * TILE_SIZE) {
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