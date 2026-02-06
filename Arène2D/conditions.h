#pragma once

#include "Context.h"
#include <cmath>

namespace AI
{
    class Conditions
    {
    public:
        static bool IsClosePlayer(Context context)
        {
            sf::Vector2f posme;
            //if (context.targetia == 1) {
                //posme= context.GetPosition(context.npc);
            //}
            //sf::Vector2f player = context.GetPosition(context.GetPlayer());
            sf::Vector2f player = { 0.f,0.f };
            //temporary while i wait for the player
            //if (sqrt(std::pow(abs(posme.x - player.x),2) + std::pow(abs(posme.y - player.y),2)) <= 20.f) {
                return true;
            //}
            //else {
            //    return false;
            //}
        }
    };
}
