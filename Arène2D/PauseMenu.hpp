#pragma once
#include <SFML/Graphics.hpp>
#include "OptionsMenu.hpp"
#include <vector>

// On définit les états du menu pause
enum PauseState { PAUSE_MAIN, PAUSE_OPTIONS };

class PauseMenu {
public:
    PauseMenu(float width, float height);

    bool init(const std::string& fontPath);
    int handleInput(sf::RenderWindow& window, const sf::Event& event);
    void update();
    void draw(sf::RenderWindow& window);

private:
    float m_width;
    float m_height;

    PauseState m_state;
    int m_selectedItemIndex;

    sf::Font m_font;
    sf::RectangleShape m_background;

    // SFML 3 : Le titre doit être membre pour être initialisé correctement
    sf::Text m_title;
    std::vector<sf::Text> m_buttons;

    OptionsMenu m_optionsMenu;
};