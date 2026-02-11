#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


class Context
{
public:
    Npc* npc;
    //Player* player
    
    // Input
    sf::Vector2f GetPosition(Npc player) {
        return player.getPosition();
    }
    sf::Vector2f GetPositionP(Player player) {
        return player.getPosition();
    }
    //Player GetPlayer(){ donc pas besoin?
    //  return Player player
    // }
    // 
    // Map

};
