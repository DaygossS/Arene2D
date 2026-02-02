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

using namespace OrcAI;

class Npc
{
public:
    int value = 100;

    FSM::StateMachine<Context> fsm;

    Context context{};

    void Init()
    {
        FightState* fightState = fsm.CreateState<FightState>();
        RoamState* roamState = fsm.CreateState<RoamState>();

        roamState->AddTransition(Conditions::IsSeeingPlayer, fightState);

        fightState->AddTransition([](const Context _context)
            {
                return !Conditions::IsSeeingPlayer(_context);
            }, roamState);

        fsm.Init(roamState, context);
    }

    void Update()
    {
        fsm.Update(context);
    }
};
