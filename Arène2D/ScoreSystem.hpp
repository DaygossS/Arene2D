#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>

class ScoreSystem {
public:
    ScoreSystem();

    bool init(const std::string& fontPath);
    void update(float deltaTime);
    void addKill();
    void draw(sf::RenderWindow& window);
    void reset();

    int getScore() const {
        int score = BASE_SCORE + (m_enemiesKilled * POINTS_PER_KILL) - static_cast<int>(m_survivalTime * POINTS_LOST_PER_SEC);
        return std::max(0, score);
    }

private:
    sf::Font m_font;
    sf::Text m_text;

    float m_survivalTime;
    int m_enemiesKilled;

    const int POINTS_PER_KILL = 500;
    const int POINTS_LOST_PER_SEC = 2;
    const int BASE_SCORE = 1000;
};