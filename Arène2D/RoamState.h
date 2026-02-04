#pragma once

#include <iostream>
#include <ostream>

#include "Context.h"
//#include "StateMachine/State.h"
#include "State.h"


namespace AI
{
    class RoamState : public FSM::State<Context>
    {
        void Enter(Context _context) override
        {
            std::cout << "Enter Roaming _State" << std::endl;
        }

        void Exit(Context _context) override
        {
            std::cout << "Exit Roaming _State" << std::endl;
        }
    };
}
