#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ScoreSystem {
public:
    ScoreSystem();

    // Charge la police (obligatoire pour afficher du texte)
    bool init(const std::string& fontPath);

    // À appeler à chaque frame pour mettre à jour le chrono
    void update(float deltaTime);

    // À appeler quand le joueur tue un ennemi
    void addKill();

    // Affiche le score
    void draw(sf::RenderWindow& window);

    // Remet tout à zéro
    void reset();

private:
    sf::Font m_font;
    sf::Text m_text;

    float m_survivalTime; // Temps écoulé en secondes
    int m_enemiesKilled;  // Compteur de kills

    // Paramètres de score
    const int POINTS_PER_KILL = 500;
    const int POINTS_LOST_PER_SEC = 2; // On perd 2 points par seconde
    const int BASE_SCORE = 1000;        // On commence avec un bonus
};