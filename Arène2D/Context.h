#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>


class Bike;

class Context
{
public:
    Bike* npc;
    Bike* npc2;
    Bike* npc3;
    
    
    // Input
    sf::Vector2f GetPosition(auto player) {
        return player.Getposition();
    }
    // 
    // Map

};
