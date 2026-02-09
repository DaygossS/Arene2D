#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class OptionsMenu {
public:
    OptionsMenu();

    bool init(const std::string& fontPath, float width, float height);
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void update();
    void draw(sf::RenderWindow& window);

    void setIsPauseMode(bool active);

    float getMusicVolume() const { return s_musicVolume; }
    float getSfxVolume() const { return s_sfxVolume; }

private:
    sf::Font m_font;
    float m_windowWidth;
    float m_windowHeight;

    bool m_isPauseMode;

    enum Tab { TAB_VOLUME, TAB_CODE };
    Tab m_currentTab;

    sf::Text m_tabVolumeText;
    sf::Text m_tabCodeText;

    static float s_musicVolume;
    static float s_sfxVolume;
    bool m_isDraggingMusic;
    bool m_isDraggingSfx;

    sf::RectangleShape m_musicTrack;
    sf::CircleShape m_musicHandle;
    sf::Text m_musicLabel;

    sf::RectangleShape m_sfxTrack;
    sf::CircleShape m_sfxHandle;
    sf::Text m_sfxLabel;

    std::string m_inputString;
    sf::Text m_inputText;
    sf::RectangleShape m_inputBox;

    sf::Text m_validateBtnText;
    sf::RectangleShape m_validateBtnShape;
    sf::Text m_feedbackText;

    sf::Text m_backButton;

    void alignSlider(float volume, sf::RectangleShape& track, sf::CircleShape& handle);
    void updateSliderValue(float mouseX, float& volume, const sf::RectangleShape& track);
};