#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>

// --- Configuration ---
const int TILE_SIZE = 32;       // Taille d'une tuile en pixels (32x32)
const int MAP_WIDTH = 50;       // Largeur de la carte en nombre de tuiles
const int MAP_HEIGHT = 28;      // Hauteur de la carte
const int TEXTURE_COLS = 4;     // Nombre de colonnes dans ton tileset (image source)


sf::Texture createTestTexture() {
    sf::Image img;
    // Crée une image de 128x32 (4 tuiles de 32px de large)
    img.resize({ static_cast<unsigned int>(TILE_SIZE * TEXTURE_COLS), static_cast<unsigned int>(TILE_SIZE) });

    // Tuile 0: Noir (Vide/Gomme)
    for (int x = 0; x < TILE_SIZE; x++) for (int y = 0; y < TILE_SIZE; y++) img.setPixel({ (unsigned)x, (unsigned)y }, sf::Color::Black);
    // Tuile 1: Vert (Herbe)
    for (int x = 32; x < 64; x++) for (int y = 0; y < TILE_SIZE; y++) img.setPixel({ (unsigned)x, (unsigned)y }, sf::Color::Green);
    // Tuile 2: Gris (Pierre)
    for (int x = 64; x < 96; x++) for (int y = 0; y < TILE_SIZE; y++) img.setPixel({ (unsigned)x, (unsigned)y }, sf::Color(100, 100, 100));
    // Tuile 3: Marron (Terre)
    for (int x = 96; x < 128; x++) for (int y = 0; y < TILE_SIZE; y++) img.setPixel({ (unsigned)x, (unsigned)y }, sf::Color(139, 69, 19));

    sf::Texture tex;
    tex.loadFromImage(img);
    return tex;
}

// Fonction pour sauvegarder la carte
void saveMap(const std::vector<int>& mapData, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                // On écrit l'ID de la tuile suivi d'un espace
                file << mapData[y * MAP_WIDTH + x] << " ";
            }
            file << "\n"; // Retour à la ligne à la fin de chaque rangée
        }
        std::cout << "Carte sauvegardee dans " << filename << std::endl;
    }
    else {
        std::cerr << "Erreur : Impossible de creer le fichier." << std::endl;
    }
}

int main() {
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "Editeur de Decor - SFML 3");
    window.setFramerateLimit(60);

    // Chargement des ressources
    // Dans un vrai projet : texture.loadFromFile("tileset.png");
    sf::Texture tileset = createTestTexture();
    sf::Sprite sprite(tileset);

    // Données de la carte (initialisée à 0)
    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);

    // État de l'éditeur
    int currentTileID = 1; // L'ID de la tuile qu'on va peindre (1 = Vert par défaut)

    while (window.isOpen()) {
        // --- Gestion des événements (Syntaxe SFML 3) ---
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Changement de tuile avec la molette
            else if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (wheel->delta > 0) currentTileID++;
                else currentTileID--;

                // Garde-fou pour rester dans les limites des IDs disponibles (0 à 3 ici)
                if (currentTileID >= TEXTURE_COLS) currentTileID = 0;
                if (currentTileID < 0) currentTileID = TEXTURE_COLS - 1;

                std::cout << "Tuile selectionnee : " << currentTileID << std::endl;
            }
            // Sauvegarde avec la touche 'S'
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::S) {
                    saveMap(map, "niveau.txt");
                }
            }
        }

        // --- Logique (Peinture) ---
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            // Récupérer la position de la souris relative à la fenêtre
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Convertir en coordonnées grille
            int gridX = mousePos.x / TILE_SIZE;
            int gridY = mousePos.y / TILE_SIZE;

            // Vérifier qu'on est bien dans la fenêtre
            if (gridX >= 0 && gridX < MAP_WIDTH && gridY >= 0 && gridY < MAP_HEIGHT) {
                map[gridY * MAP_WIDTH + gridX] = currentTileID;
            }
        }

        // --- Rendu ---
        window.clear(sf::Color(50, 50, 50)); // Fond gris foncé

        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int x = 0; x < MAP_WIDTH; ++x) {
                int tileID = map[y * MAP_WIDTH + x];

                // Calcul de la position de la tuile dans la texture (Tileset)
                // Hypothèse : Le tileset est une seule ligne horizontale de tuiles
                int tuileSourceX = tileID * TILE_SIZE;
                int tuileSourceY = 0;

                sprite.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE) });

                // On découpe la bonne partie de la texture
                sprite.setTextureRect(sf::IntRect({ tuileSourceX, tuileSourceY }, { TILE_SIZE, TILE_SIZE }));

                window.draw(sprite);
            }
        }

        // Petit indicateur visuel de la tuile sélectionnée (suit la souris)
        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        sf::Sprite cursorSprite = sprite;
        cursorSprite.setPosition({ (float)mPos.x + 10, (float)mPos.y + 10 });
        cursorSprite.setTextureRect(sf::IntRect({ currentTileID * TILE_SIZE, 0 }, { TILE_SIZE, TILE_SIZE }));
        cursorSprite.setScale({ 0.5f, 0.5f }); // Plus petit
        window.draw(cursorSprite);

        window.display();
    }

    return 0;
}