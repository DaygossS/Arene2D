#pragma once
#include "StateMachine.h"

//#include "NpcStates/ChaseState.h"
//#include "NpcStates/Conditions.h"
//#include "NpcStates/Context.h"
//#include "NpcStates/PatrolState.h"
#include "Conditions.h"
#include "Context.h"
#include "FightState.h"
#include "RoamState.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace AI;

class Npc
{
private:
    sf::Vector2f pos = {800.f,0.f};
    sf::Sprite sprite;
    float m_speed;

    int act1 = 0;
    int act2 = 0;
    int act3 = 0;

public:
    int value = 100;

    sf::Vector2f m_velocity;
    FSM::StateMachine<Context> fsm;

    PlayerTextures textures;
    Context context{};

    sf::Vector2f getPosition() {
        return sprite.getPosition();
    }
    sf::Vector2f getSize() const { return sprite.getScale(); }
    sf::Vector2f getVelocity() const { return m_velocity; }

    Npc(float startX, float startY, const PlayerTextures& texture);

    void draw(sf::RenderWindow& window);

    void Init()
    {

        

        FightState* fightState = fsm.CreateState<FightState>();
        RoamState* roamState = fsm.CreateState<RoamState>();

        roamState->AddTransition(Conditions::IsClosePlayer(_context, ), fightState);

        fightState->AddTransition([](const Context _context)
            {
                return !Conditions::IsClosePlayer(_context, *this);
            }, roamState);

        fsm.Init(roamState, context);
    }

    void update(float deltaTime, std::vector<int> collisions, TrailSystem &trailMask, TrailSystem &trailMask2)
    {
        fsm.Update(context);

        ////boucle: 1324 bghd

        if (fsm.currentState == fsm.states[1]) {
            int choice = rand() % 90;
            if (choice == 1 && (m_velocity.y == 0.f) && !((act1 == 4 && act2 == 2 && act3 == 3) || (act1 == 3 && act2 == 2 && act3 == 4))) {
                m_velocity.y = -1.f;
                m_velocity.x = 0.f;
                act3 = act2;
                act2 = act1;
                act1 = choice;
            }
            else if (choice == 2 && (m_velocity.y == 0.f) && !((act1 == 4 && act2 == 1 && act3 == 3) || (act1 == 3 && act2 == 1 && act3 == 4))) {
                m_velocity.y = 1.f;
                m_velocity.x = 0.f;
                act3 = act2;
                act2 = act1;
                act1 = choice;
            }
            else if (choice == 3 && (m_velocity.x == 0.f) && !((act1 == 2 && act2 == 4 && act3 == 1) || (act1 == 1 && act2 == 4 && act3 == 2))) {
                m_velocity.x = -1.f;
                m_velocity.y = 0.f;
                act3 = act2;
                act2 = act1;
                act1 = choice;
            }
            else if (choice == 4 && (m_velocity.x == 0.f) && !((act1 == 2 && act2 == 3 && act3 == 1) || (act1 == 1 && act2 == 3 && act3 == 2))) {
                m_velocity.x = 1.f;
                m_velocity.y = 0.f;
                act3 = act2;
                act2 = act1;
                act1 = choice;
            }

        }
        
        sprite.move(m_speed * m_velocity * deltaTime);

        sf::FloatRect IaBounds = CollisionManager::getHitbox(getPosition(), 8.f);
        bool willdie = false;
        
        if (IAisCollidingWithMap(IaBounds, collisions)) {
            willdie = true;
        }
        else if (trailMask.checkCollision(IaBounds, m_velocity)) {
            willdie = true;
        }
        else if (trailMask2.checkCollision(IaBounds, m_velocity)) {
            willdie = true;
        }


        int trie = 0;
        while (willdie && trie <3) {
            sprite.move(-m_speed * m_velocity * deltaTime);

            if (m_velocity.y == 1.f) {
               
                m_velocity.x = 1.f;
                m_velocity.y = 0.f;
            }
            else if (m_velocity.x == 1.f) {
                m_velocity.x = 0.f;
                m_velocity.y = -1.f;
            }
            else if (m_velocity.y == -1.f) {
                m_velocity.x = -1.f;
                m_velocity.y = 0.f;
            }
            else if (m_velocity.x == -1.f) {
                m_velocity.x = 0.f;
                m_velocity.y = 1.f;
            }

            sprite.move(m_speed * m_velocity * deltaTime);

            if (IAisCollidingWithMap(IaBounds, collisions)) {
                willdie = true;
            }
            else if (trailMask.checkCollision(IaBounds, m_velocity)) {
                willdie = true;
            }
            else if (trailMask2.checkCollision(IaBounds, m_velocity)) {
                willdie = true;
            }
            else { willdie = false; }

            trie++;
        }

        
    }

    void reset(float startX, float startY);
    void updateOrigin();
};





//#include "SFML/Graphics.hpp"
//
//class Player
//{
//public:
//
//    Player(float startX, float startY);
//
//
//    void handleInput();
//    void update(float deltaTime);
//    void draw(sf::RenderWindow& window);
//
//    sf::Vector2f getPosition() const { return m_shape.getPosition(); }
//    sf::Vector2f getSize() const { return m_shape.getSize(); }
//    sf::Vector2f getVelocity() const { return m_velocity; }
//private:
//    sf::RectangleShape m_shape;
//    sf::Vector2f m_velocity;
//    float m_speed;
//};

