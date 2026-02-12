#include "Bike.h"
#include <iostream>
#include <cmath> 

const int MAP_WIDTH_NPC = 50;
const int MAP_HEIGHT_NPC = 28;
const int TILE_SIZE_NPC = 32;

Npc::Npc(float startX, float startY, const PlayerTextures& texture)
    : textures(texture), sprite(*texture.right)
{
    activate = false;
    m_speed = 300.f;
    m_velocity = { 0.f, 0.f };

    sprite.setTexture(*textures.right, true);
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
    activate = false;
    sprite.setPosition({ startX, startY });
    m_velocity = { 0.f, 0.f };
    sprite.setTexture(*textures.right, true);
    updateOrigin();
}

void Npc::updateOrigin() {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}

bool isPositionSafe(sf::Vector2f pos, const std::vector<int>& collisions, TrailSystem& t1, TrailSystem& t2, TrailSystem& t3, sf::Vector2f velocity) {
    sf::FloatRect bounds = CollisionManager::getHitbox(pos, 6.f);

    if (CollisionManager::checkMapCollision(bounds, collisions, MAP_WIDTH_NPC, MAP_HEIGHT_NPC, TILE_SIZE_NPC)) return false;
    if (t1.checkCollision(bounds, velocity)) return false;
    if (t2.checkCollision(bounds, velocity)) return false;
    if (t3.checkCollision(bounds, velocity)) return false;

    return true;
}

void Npc::update(float deltaTime, std::vector<int> collisions, TrailSystem& trailMask, TrailSystem& trailMask2, TrailSystem& trailMask3)
{
    if (!activate) return;

    fsm.Update(context);

    if (m_velocity.x == 0 && m_velocity.y == 0) {
        m_velocity = { 1.f, 0.f };
        sprite.setTexture(*textures.right, true);
        updateOrigin();
    }

    sf::Vector2f currentPos = getPosition();

    float lookAheadDist = 40.f;
    sf::Vector2f futurePos = currentPos + (m_velocity * lookAheadDist);

    bool dangerAhead = !isPositionSafe(futurePos, collisions, trailMask, trailMask2, trailMask3, m_velocity);

    sf::Vector2f newVelocity = m_velocity;
    bool changeDir = false;

    if (dangerAhead || (rand() % 200 == 0)) {

        sf::Vector2f leftTurn, rightTurn;
        const sf::Texture* texLeft = nullptr;
        const sf::Texture* texRight = nullptr;

        if (m_velocity.x != 0) {
            leftTurn = { 0.f, -1.f };
            rightTurn = { 0.f, 1.f };
            texLeft = textures.up;
            texRight = textures.down;
        }
        else {
            leftTurn = { -1.f, 0.f };
            rightTurn = { 1.f, 0.f };
            texLeft = textures.left;
            texRight = textures.right;
        }

        bool canGoLeft = isPositionSafe(currentPos + (leftTurn * lookAheadDist), collisions, trailMask, trailMask2, trailMask3, leftTurn);
        bool canGoRight = isPositionSafe(currentPos + (rightTurn * lookAheadDist), collisions, trailMask, trailMask2, trailMask3, rightTurn);

        if (canGoLeft && canGoRight) {
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
    }

    if (changeDir) {
        updateOrigin();
    }

    m_velocity = newVelocity;

    sprite.move(m_speed * m_velocity * deltaTime);

    sf::FloatRect bounds = CollisionManager::getHitbox(getPosition(), 8.f);
    bool dead = false;

    if (CollisionManager::checkMapCollision(bounds, collisions, MAP_WIDTH_NPC, MAP_HEIGHT_NPC, TILE_SIZE_NPC)) dead = true;
    else if (trailMask.checkCollision(bounds, m_velocity)) dead = true;
    else if (trailMask2.checkCollision(bounds, m_velocity)) dead = true;
    else if (trailMask3.checkCollision(bounds, m_velocity)) dead = true;

    if (dead) {
        sprite.move(-m_speed * m_velocity * deltaTime);
    }
}