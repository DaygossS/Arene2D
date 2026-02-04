#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "WorldAssets.hpp"
#include "Player.hpp"

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;
const int TEXTURE_COLS = 5;

// --- FONCTIONS UTILITAIRES ---

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
    file >> tempStr; // Pour sauter le séparateur "---"
    count = 0;
    while (count < colData.size() && file >> val) {
        colData[count] = val;
        count++;
    }
}

// Radar pour les MURS (basé sur la grille)
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
            return true; // Sortie de carte
        }
    }
    return false;
}


// On ajoute le paramètre "velocity" pour savoir où on va
bool isCollidingWithTrail(const sf::FloatRect& bounds, const sf::Vector2f& velocity, const sf::Image& mask) {

    // On définit une petite marge pour ne pas que les coins accrochent trop
    float margin = 2.f;

    // On va stocker les points à vérifier ici
    std::vector<sf::Vector2f> checkPoints;

    // LOGIQUE DE DIRECTION :
    // On regarde seulement le côté vers lequel on avance.

    // 1. On avance vers la DROITE (X positif)
    if (velocity.x > 0) {
        // On vérifie le coin Haut-Droit et Bas-Droit
        checkPoints.push_back({ bounds.position.x + bounds.size.x, bounds.position.y + margin });
        checkPoints.push_back({ bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y - margin });
    }
    // 2. On avance vers la GAUCHE (X négatif)
    else if (velocity.x < 0) {
        // On vérifie le coin Haut-Gauche et Bas-Gauche
        checkPoints.push_back({ bounds.position.x, bounds.position.y + margin });
        checkPoints.push_back({ bounds.position.x, bounds.position.y + bounds.size.y - margin });
    }
    // 3. On avance vers le BAS (Y positif)
    else if (velocity.y > 0) {
        // On vérifie le coin Bas-Gauche et Bas-Droit
        checkPoints.push_back({ bounds.position.x + margin, bounds.position.y + bounds.size.y });
        checkPoints.push_back({ bounds.position.x + bounds.size.x - margin, bounds.position.y + bounds.size.y });
    }
    // 4. On avance vers le HAUT (Y négatif)
    else if (velocity.y < 0) {
        // On vérifie le coin Haut-Gauche et Haut-Droit
        checkPoints.push_back({ bounds.position.x + margin, bounds.position.y });
        checkPoints.push_back({ bounds.position.x + bounds.size.x - margin, bounds.position.y });
    }

    // Maintenant on teste seulement ces points précis
    sf::Vector2u maskSize = mask.getSize();
    for (const auto& point : checkPoints) {
        unsigned int x = (unsigned int)point.x;
        unsigned int y = (unsigned int)point.y;

        if (x < maskSize.x && y < maskSize.y) {
            if (mask.getPixel({ x, y }) == sf::Color::White) {
                return true;
            }
        }
    }
    return false;
}
// --- MAIN ---

int main() {
    sf::RenderWindow window(sf::VideoMode({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }), "TRON GAME");
    window.setFramerateLimit(60);

    // 1. Visuel Murs/Sol
    sf::Texture tileset = createTronTexture(TILE_SIZE, TEXTURE_COLS);
    sf::Sprite mapSprite(tileset);

    // 2. Visuel Trainée (GPU)
    sf::RenderTexture trailTexture;
    trailTexture.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) });
    trailTexture.clear(sf::Color::Transparent);
    sf::Sprite trailDisplay(trailTexture.getTexture());

    // 3. Logique Trainée (CPU - "Le Carnet de notes")
    sf::Image trailMask;
    trailMask.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);

    // Chargement Niveau
    std::vector<int> map(MAP_WIDTH * MAP_HEIGHT, 0);
    std::vector<int> collisions(MAP_WIDTH * MAP_HEIGHT, 0);
    // Assure-toi que "niveau5.txt" existe bien !
    openMap(map, collisions, "niveau5.txt");

    // Joueur
    Player player(100.f, 300.f);

    // Pinceau
    sf::RectangleShape trailBrush;
    trailBrush.setFillColor(sf::Color::White); // Important : Cyan pour correspondre à la détection

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) window.close();
            }
        }

        player.handleInput();
        player.update(deltaTime);

        // --- VERIFICATIONS COLLISIONS (Avant de dessiner !) ---

        sf::FloatRect playerBounds(player.getPosition(), player.getSize());
        bool isDead = false;

        // A. Collision Mur
        if (isCollidingWithMap(playerBounds, collisions)) {
            isDead = true;
        }
        // B. Collision Trainée
        else if (isCollidingWithTrail(playerBounds,player.getVelocity(), trailMask)) {
            isDead = true;
        }

        if (isDead) {
            // Reset position
            player = Player(100.f, 300.f);

            // Reset Visuel (GPU)
            trailTexture.clear(sf::Color::Transparent);

            // Reset Mémoire (CPU) - On remet tout en noir
            trailMask.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);

            // On saute le reste de la boucle pour ne pas dessiner une frame "morte"
            continue;
        }

        // --- MISE A JOUR DE LA TRAINEE (Si vivant) ---

        // 1. Dessin GPU (Ecran)
        trailBrush.setSize({ player.getSize().x - 2, player.getSize().y - 2 });
        trailBrush.setPosition({ player.getPosition().x + 1, player.getPosition().y + 1 });
        trailTexture.draw(trailBrush);
        trailTexture.display();

        // 2. Dessin CPU (Mémoire)
        int startX = (int)(player.getPosition().x + 1);
        int startY = (int)(player.getPosition().y + 1);
        int endX = startX + (int)(player.getSize().x - 2);
        int endY = startY + (int)(player.getSize().y - 2);

        sf::Vector2u maskSize = trailMask.getSize();

        
        for (int x = startX; x < endX; x++) {
            for (int y = startY; y < endY; y++) {
                if (x >= 0 && x < (int)maskSize.x && y >= 0 && y < (int)maskSize.y) {
                    
                    trailMask.setPixel({ (unsigned int)x, (unsigned int)y }, sf::Color::White);
                }
            }
        }

        // --- RENDU FINAL ---
        window.clear(sf::Color::Black);

        // 1. Carte
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

        // 2. Trainée
        window.draw(trailDisplay);

        // 3. Joueur
        player.draw(window);

        window.display();
    }
    return 0;
}