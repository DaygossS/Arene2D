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

    bool getspeedy() const { return speedy; }
    bool getslow() const { return slow; }
    bool getrainbow() const { return rainbow; }
    bool getSpecialSkin() const { return specialSkin; }

private:
    void updateSliderValue(float mouseX, float& volumeVar, const sf::RectangleShape& track);

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
    sf::Text m_feedbackText;

    sf::Text m_backButton;

    bool speedy = false;
    bool slow = false;
    bool rainbow = false;
    bool specialSkin = false;
};