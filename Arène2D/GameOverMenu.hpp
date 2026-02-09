#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Pour savoir quelle action le joueur a choisie
enum class GameOverAction {
    None,       // Rien ne se passe
    Restart,    // Le joueur veut rejouer
    BackToMenu  // Retour au menu principal
};

class GameOverMenu {
public:
    GameOverMenu();

    // Initialisation (chargement font, positionnement)
    bool init(const std::string& fontPath, float width, float height);

    // À appeler quand la partie est finie pour définir le texte (Gagné ou Perdu)
    void setState(bool isVictory, int score = 0);

    // Gestion des clics
    GameOverAction handleEvent(const sf::Event& event, sf::RenderWindow& window);

    // Affichage
    void draw(sf::RenderWindow& window);

private:
    sf::Font m_font;
    float m_width, m_height;

    // Fond semi-transparent
    sf::RectangleShape m_background;

    // Textes
    sf::Text m_titleText;   // "VICTOIRE" ou "DÉFAITE"
    sf::Text m_scoreText;   // "Score : 100"

    sf::Text m_btnRestart;
    sf::Text m_btnMenu;
};