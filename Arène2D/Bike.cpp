#include "Bike.h"
#include <iostream>

// Constantes de la map
const int MAP_WIDTH_NPC = 50;
const int MAP_HEIGHT_NPC = 28;
const int TILE_SIZE_NPC = 32;

Npc::Npc(float startX, float startY, const PlayerTextures& texture)
    : textures(texture), sprite(texture.right)
{
    // --- INITIALISATION EXPLICTE ---
    activate = false;  // <--- C'est ici la correction : on force l'arrêt au début
    m_speed = 300.f;
    m_velocity = { 0.f, 0.f };

    sprite.setTexture(textures.right, true);
    sprite.setScale({ 0.3f, 0.3f });
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
    activate = false; // On arrête le NPC quand on reset
    sprite.setPosition({ startX, startY });
    m_velocity = { 0.f, 0.f };
    sprite.setTexture(textures.left, true);
    updateOrigin();
}

void Npc::updateOrigin() {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}

void Npc::update(float deltaTime, std::vector<int> collisions, TrailSystem& trailMask, TrailSystem& trailMask2, TrailSystem& trailMask3)
{
    // Si activate est false, on ne rentre pas ici, donc pas de mouvement
    if (activate) {
        fsm.Update(context);

        int choice = rand() % 90;

        // --- DIRECTION ALEATOIRE + CHANGEMENT DE TEXTURE ---
        if (choice == 1 && (m_velocity.y == 0.f) && !((act1 == 4 && act2 == 2 && act3 == 3) || (act1 == 3 && act2 == 2 && act3 == 4))) {
            m_velocity.y = -1.f; m_velocity.x = 0.f;
            act3 = act2; act2 = act1; act1 = choice;
            sprite.setTexture(textures.up, true);
            updateOrigin();
        }
        else if (choice == 2 && (m_velocity.y == 0.f) && !((act1 == 4 && act2 == 1 && act3 == 3) || (act1 == 3 && act2 == 1 && act3 == 4))) {
            m_velocity.y = 1.f; m_velocity.x = 0.f;
            act3 = act2; act2 = act1; act1 = choice;
            sprite.setTexture(textures.down, true);
            updateOrigin();
        }
        else if (choice == 3 && (m_velocity.x == 0.f) && !((act1 == 2 && act2 == 4 && act3 == 1) || (act1 == 1 && act2 == 4 && act3 == 2))) {
            m_velocity.x = -1.f; m_velocity.y = 0.f;
            act3 = act2; act2 = act1; act1 = choice;
            sprite.setTexture(textures.left, true);
            updateOrigin();
        }
        else if (choice == 4 && (m_velocity.x == 0.f) && !((act1 == 2 && act2 == 3 && act3 == 1) || (act1 == 1 && act2 == 3 && act3 == 2))) {
            m_velocity.x = 1.f; m_velocity.y = 0.f;
            act3 = act2; act2 = act1; act1 = choice;
            sprite.setTexture(textures.right, true);
            updateOrigin();
        }

        sprite.move(m_speed * m_velocity * deltaTime);

        sf::FloatRect IaBounds = CollisionManager::getHitbox(getPosition(), 8.f);
        bool willdie = false;

        // Vérifications collisions
        if (CollisionManager::checkMapCollision(IaBounds, collisions, MAP_WIDTH_NPC, MAP_HEIGHT_NPC, TILE_SIZE_NPC)) willdie = true;
        else if (trailMask.checkCollision(IaBounds, m_velocity)) willdie = true;
        else if (trailMask2.checkCollision(IaBounds, m_velocity)) willdie = true;
        else if (trailMask3.checkCollision(IaBounds, m_velocity)) willdie = true;

        int trie = 0;
        while (willdie && trie < 3) {
            sprite.move(-m_speed * m_velocity * deltaTime);

            // CORRECTION DIRECTION + TEXTURE
            if (m_velocity.y == 1.f) {
                m_velocity.x = 1.f; m_velocity.y = 0.f;
                sprite.setTexture(textures.right, true);
            }
            else if (m_velocity.x == 1.f) {
                m_velocity.x = 0.f; m_velocity.y = -1.f;
                sprite.setTexture(textures.up, true);
            }
            else if (m_velocity.y == -1.f) {
                m_velocity.x = -1.f; m_velocity.y = 0.f;
                sprite.setTexture(textures.left, true);
            }
            else if (m_velocity.x == -1.f) {
                m_velocity.x = 0.f; m_velocity.y = 1.f;
                sprite.setTexture(textures.down, true);
            }

            updateOrigin();

            sprite.move(m_speed * m_velocity * deltaTime);

            if (CollisionManager::checkMapCollision(IaBounds, collisions, MAP_WIDTH_NPC, MAP_HEIGHT_NPC, TILE_SIZE_NPC)) willdie = true;
            else if (trailMask.checkCollision(IaBounds, m_velocity)) willdie = true;
            else if (trailMask2.checkCollision(IaBounds, m_velocity)) willdie = true;
            else if (trailMask3.checkCollision(IaBounds, m_velocity)) willdie = true;
            else willdie = false;

            trie++;
        }
    }
}