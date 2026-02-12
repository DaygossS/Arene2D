#include "Bike.h"
#include <iostream>
#include <cmath> // Pour std::abs

// Constantes de la map
const int MAP_WIDTH_NPC = 50;
const int MAP_HEIGHT_NPC = 28;
const int TILE_SIZE_NPC = 32;

Npc::Npc(float startX, float startY, const PlayerTextures& texture)
    : textures(texture), sprite(texture.right)
{
    activate = false;
    m_speed = 300.f;
    m_velocity = { 0.f, 0.f };

    sprite.setTexture(textures.right, true);
    sprite.setScale({ 0.3f, 0.3f }); // Assure-toi que c'est la bonne échelle
    sprite.setPosition({ startX, startY });

    updateOrigin();
}

void Npc::Init()
{
    FightState* fightState = fsm.CreateState<FightState>();
    RoamState* roamState = fsm.CreateState<RoamState>();

    roamState->AddTransition(Conditions::IsClosePlayer, fightState);

    fightState->AddTransition([](const Context _context)
        {
            return !Conditions::IsClosePlayer(_context);
        }, roamState);

    fsm.Init(roamState, context);
}

void Npc::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Npc::reset(float startX, float startY) {
    activate = false;
    sprite.setPosition({ startX, startY });
    m_velocity = { 0.f, 0.f };
    sprite.setTexture(textures.right, true); // Texture par défaut
    updateOrigin();
}

void Npc::updateOrigin() {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}

// Fonction utilitaire locale pour vérifier si une position est sûre
bool isPositionSafe(sf::Vector2f pos, const std::vector<int>& collisions, TrailSystem& t1, TrailSystem& t2, TrailSystem& t3, sf::Vector2f velocity) {
    // On utilise une hitbox légèrement plus petite pour l'anticipation pour éviter les faux positifs dans les virages serrés
    sf::FloatRect bounds = CollisionManager::getHitbox(pos, 6.f);

    if (CollisionManager::checkMapCollision(bounds, collisions, MAP_WIDTH_NPC, MAP_HEIGHT_NPC, TILE_SIZE_NPC)) return false;
    if (t1.checkCollision(bounds, velocity)) return false; // Attention à la velocity passée ici
    if (t2.checkCollision(bounds, velocity)) return false;
    if (t3.checkCollision(bounds, velocity)) return false;

    return true;
}

void Npc::update(float deltaTime, std::vector<int> collisions, TrailSystem& trailMask, TrailSystem& trailMask2, TrailSystem& trailMask3)
{
    if (!activate) return;

    fsm.Update(context);

    // 1. Initialisation du mouvement si à l'arrêt
    if (m_velocity.x == 0 && m_velocity.y == 0) {
        m_velocity = { -1.f, 0.f }; // On démarre vers la gauche par défaut
        sprite.setTexture(textures.right, true);
        updateOrigin();
    }

    sf::Vector2f currentPos = getPosition();

    // Distance d'anticipation (Regarde devant lui)
    // Plus le chiffre est grand, plus il anticipe tôt. 40.f est un bon compromis.
    // m_speed * m_velocity * deltaTime
    float lookAheadDist = m_speed * deltaTime * 2;
    sf::Vector2f futurePos = currentPos + (m_velocity * lookAheadDist);

    bool dangerAhead = !isPositionSafe(futurePos, collisions, trailMask, trailMask2, trailMask3, m_velocity);

    // Logique de changement de direction
    sf::Vector2f newVelocity = m_velocity;
    bool changeDir = false;

    // Si danger devant OU un petit coup de folie aléatoire (pour ne pas faire que des lignes droites)
    if (dangerAhead || (rand() % 140 == 0)) {

        // Calcul des directions possibles (Gauche et Droite relatives)
        sf::Vector2f leftTurn, rightTurn;
        const sf::Texture* texLeft = nullptr;
        const sf::Texture* texRight = nullptr;

        if (m_velocity.x != 0) { // On bouge horizontalement
            leftTurn = { 0.f, -1.f }; // Haut
            rightTurn = { 0.f, 1.f }; // Bas
            texLeft = &textures.up;
            texRight = &textures.down;
        }
        else { // On bouge verticalement
            leftTurn = { -1.f, 0.f }; // Gauche
            rightTurn = { 1.f, 0.f }; // Droite
            texLeft = &textures.left;
            texRight = &textures.right;
        }

        // Vérifier si ces directions sont sûres (un peu devant)
        bool canGoLeft = isPositionSafe(currentPos + (leftTurn * lookAheadDist), collisions, trailMask, trailMask2, trailMask3, leftTurn);
        bool canGoRight = isPositionSafe(currentPos + (rightTurn * lookAheadDist), collisions, trailMask, trailMask2, trailMask3, rightTurn);

        if (canGoLeft && canGoRight) {
            // Si les deux sont libres, choix aléatoire
            if (rand() % 2 == 0) {
                newVelocity = leftTurn;
                sprite.setTexture(*texLeft, true);
            }
            else {
                newVelocity = rightTurn;
                sprite.setTexture(*texRight, true);
            }
            changeDir = true;
        }
        else if (canGoLeft) {
            newVelocity = leftTurn;
            sprite.setTexture(*texLeft, true);
            changeDir = true;
        }
        else if (canGoRight) {
            newVelocity = rightTurn;
            sprite.setTexture(*texRight, true);
            changeDir = true;
        }
        // Si aucune n'est libre... on garde la direction actuelle et on prie (ou on meurt au prochain tour)
    }

    if (changeDir) {
        updateOrigin();
    }

    // Mise à jour de la vélocité finale
    m_velocity = newVelocity;

    // Déplacement
    sprite.move(m_speed * m_velocity * deltaTime);

    // --- Vérification finale de collision (Mort réelle) ---
    // C'est ici qu'on meurt vraiment si on touche quelque chose malgré l'IA
    sf::FloatRect bounds = CollisionManager::getHitbox(getPosition(), 8.f);
    bool dead = false;

    // Note: on passe m_velocity pour éviter de se collisionner avec sa propre trainée fraîchement posée
    if (CollisionManager::checkMapCollision(bounds, collisions, MAP_WIDTH_NPC, MAP_HEIGHT_NPC, TILE_SIZE_NPC)) dead = true;
    else if (trailMask.checkCollision(bounds, m_velocity)) dead = true;
    else if (trailMask2.checkCollision(bounds, m_velocity)) dead = true;
    else if (trailMask3.checkCollision(bounds, m_velocity)) dead = true;

    // Si collision fatale, on tente une micro correction de dernière seconde (Hitbox clipping)
    if (dead) {
        // Annuler le mouvement pour ne pas rester coincé dans le mur visuellement
        sprite.move(-m_speed * m_velocity * deltaTime);

        
    }
}