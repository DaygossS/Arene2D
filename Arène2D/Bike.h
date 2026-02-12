#pragma once
#include "StateMachine.h"
#include "Conditions.h"
#include "Context.h"
#include "FightState.h"
#include "RoamState.h"
#include "Player.hpp"       
#include "TrailSystem.hpp"  
#include "CollisionManager.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

using namespace AI;

class Npc
{
private:
    sf::Vector2f pos = { 800.f, 0.f };
    sf::Sprite sprite;
    float m_speed;

    int act1 = 0;
    int act2 = 0;
    int act3 = 0;

    bool activate = false;

public:
    int value = 100;

    sf::Vector2f m_velocity;
    FSM::StateMachine<Context> fsm;

    PlayerTextures textures;
    Context context{};

    // Fonctions inline simples (ok dans le .h)
    sf::Vector2f getPosition() { return sprite.getPosition(); }
    sf::Vector2f getSize() const { return sprite.getScale(); }
    sf::Vector2f getVelocity() const { return m_velocity; }
    bool getActive() const { return activate; }
    void setActie(bool a) { activate = a; }
    void setSpeed(float newfast) { m_speed = newfast; }

    // --- SEULEMENT LES PROTOTYPES ICI (Pas de code) ---

    // Constructeur
    Npc(float startX, float startY, const PlayerTextures& texture);

    // Méthodes
    void draw(sf::RenderWindow& window);
    void Init();
    void update(float deltaTime, std::vector<int> collisions, TrailSystem& trailMask, TrailSystem& trailMask2, TrailSystem& trailMask3);
    void reset(float startX, float startY);
    void updateOrigin();
};