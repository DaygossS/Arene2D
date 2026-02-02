#pragma once
#include "StateMachine.h"

//#include "NpcStates/ChaseState.h"
//#include "NpcStates/Conditions.h"
//#include "NpcStates/Context.h"
//#include "NpcStates/PatrolState.h"
#include "Conditions.h"
#include "Context.h"

using namespace OrcAI;

class Npc
{
public:
    int value = 100;

    FSM::StateMachine<Context> fsm;

    Context context{};

    void Init()
    {
        //PatrolState* patrolState = fsm.CreateState<PatrolState>();
        //ChaseState* chaseState = fsm.CreateState<ChaseState>();

        //patrolState->AddTransition(Conditions::IsSeeingPlayer, chaseState);

        //chaseState->AddTransition([](const NpcContext _context)
        //    {
        //        return !Conditions::IsSeeingPlayer(_context);
        //    }, patrolState);

        //fsm.Init(patrolState, context);
    }

    void Update()
    {
        fsm.Update(context);
    }
};
