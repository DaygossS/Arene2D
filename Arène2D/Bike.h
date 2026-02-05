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
    //sf::Vector2f pos = {0.f,0.f};
    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;
    float m_speed;
public:
    int value = 100;

    FSM::StateMachine<Context> fsm;

    Context context{};

    sf::Vector2f GetPosition() {
        return m_shape.getPosition();
    }
    sf::Vector2f getSize() const { return m_shape.getSize(); }
    sf::Vector2f getVelocity() const { return m_velocity; }

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

    void Update()
    {
        fsm.Update(context);
    }
};


