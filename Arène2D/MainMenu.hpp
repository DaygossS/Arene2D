#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "OptionsMenu.hpp" // <--- On l'inclut ici, plus dans le main

class MainMenu {
public:
    MainMenu(float width, float height);

    bool init(const std::string& fontPath);

    void draw(sf::RenderWindow& window);

    // Retourne : 0 = Reste dans le menu, 1 = Jouer, 3 = Quitter
    // Note : Le return 2 (Options) disparaît car c'est géré en interne !
    int handleInput(sf::RenderWindow& window, const sf::Event& event);

private:
    sf::Font m_font;
    std::vector<sf::Text> m_options;
    int m_selectedItemIndex = -1;

    // --- NOUVEAU ---
    OptionsMenu m_optionsMenu; // L'instance du menu options est DANS le menu principal
    bool m_showOptions;        // Est-ce qu'on affiche les options ?

    float m_width;  // On garde la taille pour l'init des options
    float m_height;
};