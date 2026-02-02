#pragma once

#include <iostream>
#include <ostream>

#include "Context.h"
#include "StateMachine/State.h"

namespace OrcAI
{
    class PatrolState : public FSM::State<NpcContext>
    {
        void Enter(NpcContext _context) override
        {
            std::cout << "Enter Patrol _State" << std::endl;
        }

        void Exit(NpcContext _context) override
        {
            std::cout << "Exit Patrol _State" << std::endl;
        }
    };
}
