#include "OptionsMenu.hpp"
#include <iostream>
#include <cmath> 

static sf::Color Gris = sf::Color(100, 100, 100);

float OptionsMenu::s_musicVolume = 0.5f;
float OptionsMenu::s_sfxVolume = 0.5f;

OptionsMenu::OptionsMenu()
    : m_windowWidth(0.f), m_windowHeight(0.f),
    m_isPauseMode(false),
    m_currentTab(TAB_VOLUME),
    m_isDraggingMusic(false), m_isDraggingSfx(false),
    m_tabVolumeText(m_font),
    m_tabCodeText(m_font),
    m_musicLabel(m_font),
    m_sfxLabel(m_font),
    m_inputText(m_font),
    m_validateBtnText(m_font),
    m_feedbackText(m_font),
    m_backButton(m_font)
{
}

bool OptionsMenu::init(const std::string& fontPath, float width, float height) {
    m_windowWidth = width;
    m_windowHeight = height;

    if (!m_font.openFromFile("../Assets/Fonts/arial.ttf")) return false;

    m_tabVolumeText.setString("VOLUME");
    m_tabVolumeText.setCharacterSize(30);
    m_tabVolumeText.setPosition({ width * 0.3f, 50.f });

    m_tabCodeText.setString("CODE");
    m_tabCodeText.setCharacterSize(30);
    m_tabCodeText.setPosition({ width * 0.7f, 50.f });

    m_musicTrack.setSize({ 300.f, 5.f });
    m_musicTrack.setFillColor(sf::Color(100, 100, 100));
    m_musicTrack.setPosition({ (width - 300.f) / 2.f, 200.f });

    m_musicHandle.setRadius(10.f);
    m_musicHandle.setOrigin({ 10.f, 10.f });
    m_musicHandle.setFillColor(sf::Color::White);
    alignSlider(s_musicVolume, m_musicTrack, m_musicHandle);

    m_musicLabel.setCharacterSize(24);
    m_musicLabel.setPosition({ m_musicTrack.getPosition().x, 160.f });

    m_sfxTrack.setSize({ 300.f, 5.f });
    m_sfxTrack.setFillColor(sf::Color(100, 100, 100));
    m_sfxTrack.setPosition({ (width - 300.f) / 2.f, 350.f });

    m_sfxHandle.setRadius(10.f);
    m_sfxHandle.setOrigin({ 10.f, 10.f });
    m_sfxHandle.setFillColor(sf::Color::White);
    alignSlider(s_sfxVolume, m_sfxTrack, m_sfxHandle);

    m_sfxLabel.setCharacterSize(24);
    m_sfxLabel.setPosition({ m_sfxTrack.getPosition().x, 310.f });

    m_inputBox.setSize({ 300.f, 40.f });
    m_inputBox.setFillColor(sf::Color(50, 50, 50));
    m_inputBox.setOutlineColor(sf::Color::White);
    m_inputBox.setOutlineThickness(1.f);
    m_inputBox.setPosition({ (width - 400.f) / 2.f, 250.f });

    m_inputText.setCharacterSize(24);
    m_inputText.setFillColor(sf::Color::White);
    m_inputText.setPosition({ m_inputBox.getPosition().x + 5.f, m_inputBox.getPosition().y + 5.f });

    m_validateBtnShape.setSize({ 90.f, 40.f });
    m_validateBtnShape.setFillColor(Gris);
    m_validateBtnShape.setPosition({ m_inputBox.getPosition().x + 310.f, 250.f });

    m_validateBtnText.setString("OK");
    m_validateBtnText.setCharacterSize(20);
    m_validateBtnText.setPosition({ m_validateBtnShape.getPosition().x + 30.f, m_validateBtnShape.getPosition().y + 8.f });

    m_feedbackText.setCharacterSize(20);
    m_feedbackText.setFillColor(sf::Color::Green);
    m_feedbackText.setPosition({ m_inputBox.getPosition().x, 300.f });

    m_backButton.setString("RETOUR");
    m_backButton.setCharacterSize(30);
    m_backButton.setFillColor(sf::Color::Red);
    sf::FloatRect textRect = m_backButton.getLocalBounds();
    m_backButton.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
    m_backButton.setPosition({ width / 2.f, height - 80.f });

    update();
    return true;
}

void OptionsMenu::setIsPauseMode(bool active) {
    m_isPauseMode = active;
    if (m_isPauseMode) {
        m_currentTab = TAB_VOLUME;
    }
}

void OptionsMenu::alignSlider(float volume, sf::RectangleShape& track, sf::CircleShape& handle) {
    float x = track.getPosition().x + (volume * track.getSize().x);
    handle.setPosition({ x, track.getPosition().y + 2.5f });
}

void OptionsMenu::updateSliderValue(float mouseX, float& volume, const sf::RectangleShape& track) {
    float relativeX = mouseX - track.getPosition().x;
    volume = relativeX / track.getSize().x;

    if (volume < 0.f) volume = 0.f;
    if (volume > 1.f) volume = 1.f;
}

void OptionsMenu::update() {
    m_musicLabel.setString("Musique : " + std::to_string((int)(s_musicVolume * 100)) + "%");
    m_sfxLabel.setString("SFX : " + std::to_string((int)(s_sfxVolume * 100)) + "%");

    alignSlider(s_musicVolume, m_musicTrack, m_musicHandle);
    alignSlider(s_sfxVolume, m_sfxTrack, m_sfxHandle);

    if (m_currentTab == TAB_VOLUME) {
        m_tabVolumeText.setFillColor(sf::Color::Blue);
        m_tabCodeText.setFillColor(Gris);
    }
    else {
        m_tabVolumeText.setFillColor(Gris);
        m_tabCodeText.setFillColor(sf::Color::Blue);
    }
}

bool OptionsMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {

            if (m_tabVolumeText.getGlobalBounds().contains(mousePosF)) {
                m_currentTab = TAB_VOLUME;
                m_feedbackText.setString("");
            }
            else if (!m_isPauseMode && m_tabCodeText.getGlobalBounds().contains(mousePosF)) {
                m_currentTab = TAB_CODE;
            }

            if (m_backButton.getGlobalBounds().contains(mousePosF)) {
                return true;
            }

            if (m_currentTab == TAB_VOLUME) {
                sf::FloatRect musicArea = m_musicTrack.getGlobalBounds();
                musicArea.size.y += 20;
                musicArea.position.y -= 10;

                sf::FloatRect sfxArea = m_sfxTrack.getGlobalBounds();
                sfxArea.size.y += 20;
                sfxArea.position.y -= 10;

                if (musicArea.contains(mousePosF)) m_isDraggingMusic = true;
                if (sfxArea.contains(mousePosF)) m_isDraggingSfx = true;
            }

            if (m_currentTab == TAB_CODE && !m_isPauseMode) {
                if (m_validateBtnShape.getGlobalBounds().contains(mousePosF)) {
                    if (m_inputString == "test") {
                        m_feedbackText.setString("Code Accepte : Mode test !");
                        m_feedbackText.setFillColor(sf::Color::Green);
                    }
                    else {
                        m_feedbackText.setString("Code Invalide.");
                        m_feedbackText.setFillColor(sf::Color::White);
                    }
                }
            }
        }
    }

    if (const auto* mouseRel = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseRel->button == sf::Mouse::Button::Left) {
            m_isDraggingMusic = false;
            m_isDraggingSfx = false;
        }
    }

    if (event.is<sf::Event::MouseMoved>()) {
        if (m_backButton.getGlobalBounds().contains(mousePosF)) m_backButton.setFillColor(sf::Color::Red);
        else m_backButton.setFillColor(sf::Color::White);

        if (m_currentTab == TAB_VOLUME) {
            if (m_isDraggingMusic) {
                updateSliderValue(mousePosF.x, s_musicVolume, m_musicTrack);
            }
            if (m_isDraggingSfx) {
                updateSliderValue(mousePosF.x, s_sfxVolume, m_sfxTrack);
            }
        }
    }

    if (m_currentTab == TAB_CODE && !m_isPauseMode) {
        if (const auto* textEvt = event.getIf<sf::Event::TextEntered>()) {
            if (textEvt->unicode == 8) {
                if (!m_inputString.empty()) m_inputString.pop_back();
            }
            else if (textEvt->unicode == 13) {
            }
            else if (textEvt->unicode < 128) {
                m_inputString += static_cast<char>(textEvt->unicode);
            }
            m_inputText.setString(m_inputString);
        }
    }

    update();
    return false;
}

void OptionsMenu::draw(sf::RenderWindow& window) {
    window.draw(m_tabVolumeText);

    if (!m_isPauseMode) {
        window.draw(m_tabCodeText);
    }

    window.draw(m_backButton);

    if (m_currentTab == TAB_VOLUME) {
        window.draw(m_musicLabel);
        window.draw(m_musicTrack);
        window.draw(m_musicHandle);

        window.draw(m_sfxLabel);
        window.draw(m_sfxTrack);
        window.draw(m_sfxHandle);
    }
    else if (m_currentTab == TAB_CODE && !m_isPauseMode) {
        window.draw(m_inputBox);
        window.draw(m_inputText);
        window.draw(m_validateBtnShape);
        window.draw(m_validateBtnText);
        window.draw(m_feedbackText);
    }
}