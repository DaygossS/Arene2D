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
public:
    int value = 100;

    sf::Vector2f m_velocity;
    FSM::StateMachine<Context> fsm;

    Context context{};

    sf::Vector2f GetPosition() {
        return sprite.getPosition();
    }
    sf::Vector2f getSize() const { return sprite.getScale(); }
    sf::Vector2f getVelocity() const { return m_velocity; }

    Npc(float startX, float startY, const sf::Texture& texture);

    void draw(sf::RenderWindow& window);

    void Init()
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

    void Update(float deltaTime, std::vector<int> collisions, sf::Image trailMask,sf::Image trailMask2)
    {
        fsm.Update(context);

        int choice = rand() % 30;
        if (choice==1 && (m_velocity.y != 1.f)) {
            m_velocity.y = -1.f;
            m_velocity.x = 0.f;
        }
        else if (choice == 2 && (m_velocity.y != -1.f)) {
            m_velocity.y = 1.f;
            m_velocity.x = 0.f;
        }
        else if (choice == 3 && (m_velocity.x != 1.f)) {
            m_velocity.x = -1.f;
            m_velocity.y = 0.f;
        }
        else if (choice == 4 && (m_velocity.x != -1.f)) {
            m_velocity.x = 1.f;
            m_velocity.y = 0.f;
        }


        sf::FloatRect IaBounds(GetPosition(), getSize());
        bool willdie = false;
        
        if (IAisCollidingWithMap(IaBounds, collisions)) {
            willdie = true;
        }
        else if (IAisCollidingWithTrail(IaBounds, m_velocity, trailMask)) {
            willdie = true;
        }
        else if (IAisCollidingWithTrail(IaBounds, m_velocity, trailMask2)) {
            willdie = true;
        }

        if (willdie) {
            std::cout << "i'm Dead" << std::endl;
        }

        sprite.move(m_speed * m_velocity * deltaTime);
    }
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

