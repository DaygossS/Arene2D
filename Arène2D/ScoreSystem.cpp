#include "ScoreSystem.hpp"
#include <iostream>
#include <cmath> // Pour std::max

ScoreSystem::ScoreSystem() : m_survivalTime(0.f), m_enemiesKilled(0), m_text(m_font) {

}

bool ScoreSystem::init(const std::string& fontPath) {
    if (!m_font.openFromFile("../Assets/Fonts/arial.ttf")) {
        std::cerr << "ERREUR: Impossible de charger la police " << fontPath << std::endl;
        return false;
    }

    m_text.setFont(m_font);
    m_text.setCharacterSize(24);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition({ 10.f, 10.f }); // En haut à gauche

    return true;
}

void ScoreSystem::update(float deltaTime) {
    m_survivalTime += deltaTime;

    // Base (1000) - (Temps * 2) + (Kills * 500)
    int timePenalty = (int)(m_survivalTime * POINTS_LOST_PER_SEC);
    int currentScore = BASE_SCORE - timePenalty + (m_enemiesKilled * POINTS_PER_KILL);

    // On évite les scores négatifs (optionnel)
    if (currentScore < 0) currentScore = 0;

    m_text.setString("Score: " + std::to_string(currentScore) +
        " | Temps: " + std::to_string((int)m_survivalTime) + "s" +
        " | Kills: " + std::to_string(m_enemiesKilled));
}

void ScoreSystem::addKill() {
    m_enemiesKilled++;
}

void ScoreSystem::draw(sf::RenderWindow& window) {
    window.draw(m_text);
}

void ScoreSystem::reset() {
    m_survivalTime = 0.f;
    m_enemiesKilled = 0;
}