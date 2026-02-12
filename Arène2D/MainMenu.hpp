#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "OptionsMenu.hpp"

enum MenuState {
    STATE_MAIN,
    STATE_LEVELS,
    STATE_OPTIONS
};

class MainMenu {
public:
    MainMenu(float width, float height);
    bool init(const std::string& fontPath);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    int handleInput(sf::RenderWindow& window, const sf::Event& event);

    std::string getSelectedLevelFile() const;
    int getSelectedLevelIndex() const { return m_selectedItemIndex; }

    void unlockNextLevel(int completedLevelIndex);

    OptionsMenu m_optionsMenu;

private:
    void loadProgress();
    void saveProgress();

    float m_width;
    float m_height;
    MenuState m_state;

    sf::Font m_font;

    std::vector<sf::Text> m_mainOptions;
    std::vector<sf::Text> m_levelOptions;
    std::string m_selectedLevelFile;
    sf::Text m_returnButton;

    int m_selectedItemIndex;
    int m_maxUnlockedLevel;

    sf::Texture m_gridTexture;
    sf::Sprite m_gridSprite;

    sf::Texture m_carTexture;
    sf::Sprite m_carSprite;
    sf::IntRect m_carRect;
    float m_animationTimer;
    int m_currentFrame;
};