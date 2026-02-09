#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "OptionsMenu.hpp"

enum PauseState {
    PAUSE_MAIN,
    PAUSE_OPTIONS
};

class PauseMenu {
public:
    PauseMenu(float width, float height);
    bool init(const std::string& fontPath);
    void update();
    void draw(sf::RenderWindow& window);

    // Retourne : 0 = Rien, 1 = Reprendre, 2 = Retour Menu Principal
    int handleInput(sf::RenderWindow& window, const sf::Event& event);

private:
    float m_width;
    float m_height;
    PauseState m_state;

    sf::Font m_font;
    std::vector<sf::Text> m_buttons;
    int m_selectedItemIndex;

    sf::RectangleShape m_background; // Le fond transparent
    OptionsMenu m_optionsMenu;
};