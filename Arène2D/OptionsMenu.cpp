#include "OptionsMenu.hpp"
#include <iostream>
#include <cmath> 

static sf::Color Gris = sf::Color(100, 100, 100);
static sf::Color Bleu = sf::Color(0, 50, 100);

float OptionsMenu::s_musicVolume = 0.5f;
float OptionsMenu::s_sfxVolume = 0.5f;

OptionsMenu::OptionsMenu()
    : m_windowWidth(0.f), m_windowHeight(0.f),
    m_isPauseMode(false),
    invincible(false),
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

    if (!m_font.openFromFile(fontPath)) return false;

    m_tabVolumeText.setString("VOLUME");
    m_tabVolumeText.setCharacterSize(30);
    m_tabVolumeText.setPosition({ width * 0.3f, height * 0.15f });
    m_tabVolumeText.setFillColor(sf::Color::White);

    m_tabCodeText.setString("CODE");
    m_tabCodeText.setCharacterSize(30);
    m_tabCodeText.setPosition({ width * 0.6f, height * 0.15f });
    m_tabCodeText.setFillColor(Gris);

    m_musicLabel.setString("Musique");
    m_musicLabel.setCharacterSize(24);
    m_musicLabel.setPosition({ width * 0.2f, height * 0.35f });

    m_musicTrack.setSize({ width * 0.4f, 5.f });
    m_musicTrack.setFillColor(sf::Color::White);
    m_musicTrack.setPosition({ width * 0.4f, height * 0.37f });

    m_musicHandle.setRadius(10.f);
    m_musicHandle.setFillColor(Bleu);
    m_musicHandle.setOrigin({ 10.f, 10.f });
    m_musicHandle.setPosition({
        m_musicTrack.getPosition().x + s_musicVolume * m_musicTrack.getSize().x,
        m_musicTrack.getPosition().y + 2.5f
        });

    m_sfxLabel.setString("Effets");
    m_sfxLabel.setCharacterSize(24);
    m_sfxLabel.setPosition({ width * 0.2f, height * 0.55f });

    m_sfxTrack.setSize({ width * 0.4f, 5.f });
    m_sfxTrack.setFillColor(sf::Color::White);
    m_sfxTrack.setPosition({ width * 0.4f, height * 0.57f });

    m_sfxHandle.setRadius(10.f);
    m_sfxHandle.setFillColor(Bleu);
    m_sfxHandle.setOrigin({ 10.f, 10.f });
    m_sfxHandle.setPosition({
        m_sfxTrack.getPosition().x + s_sfxVolume * m_sfxTrack.getSize().x,
        m_sfxTrack.getPosition().y + 2.5f
        });

    m_inputBox.setSize({ width * 0.4f, 40.f });
    m_inputBox.setFillColor(sf::Color(50, 50, 50));
    m_inputBox.setOutlineColor(sf::Color::White);
    m_inputBox.setOutlineThickness(2.f);
    m_inputBox.setPosition({ width * 0.3f, height * 0.4f });

    m_inputText.setCharacterSize(24);
    m_inputText.setFillColor(sf::Color::White);
    m_inputText.setPosition({ width * 0.31f, height * 0.41f });

    m_validateBtnText.setString("Valider");
    m_validateBtnText.setCharacterSize(24);
    m_validateBtnText.setFillColor(sf::Color::White);
    m_validateBtnText.setPosition({ width * 0.45f, height * 0.55f });

    m_feedbackText.setCharacterSize(20);
    m_feedbackText.setPosition({ width * 0.3f, height * 0.65f });

    m_backButton.setString("Retour");
    m_backButton.setCharacterSize(30);
    m_backButton.setFillColor(sf::Color::White);
    m_backButton.setPosition({ width * 0.05f, height * 0.9f });

    return true;
}

void OptionsMenu::setIsPauseMode(bool active) {
    m_isPauseMode = active;
    if (m_isPauseMode) {
        m_currentTab = TAB_VOLUME;
        m_tabCodeText.setFillColor(sf::Color::Transparent);
    }
    else {
        m_tabCodeText.setFillColor(Gris);
    }
    update();
}

void OptionsMenu::updateSliderValue(float mouseX, float& volumeVar, const sf::RectangleShape& track) {
    float x = track.getPosition().x;
    float width = track.getSize().x;

    float value = (mouseX - x) / width;

    if (value < 0.f) value = 0.f;
    if (value > 1.f) value = 1.f;

    volumeVar = value;
}

void OptionsMenu::update() {
    if (m_currentTab == TAB_VOLUME) {
        m_tabVolumeText.setFillColor(sf::Color::White);
        if (!m_isPauseMode) m_tabCodeText.setFillColor(Gris);

        m_musicHandle.setPosition({
            m_musicTrack.getPosition().x + s_musicVolume * m_musicTrack.getSize().x,
            m_musicTrack.getPosition().y + 2.5f
            });

        m_sfxHandle.setPosition({
            m_sfxTrack.getPosition().x + s_sfxVolume * m_sfxTrack.getSize().x,
            m_sfxTrack.getPosition().y + 2.5f
            });
    }
    else {
        m_tabVolumeText.setFillColor(Gris);
        m_tabCodeText.setFillColor(sf::Color::White);
    }
}

bool OptionsMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF((float)mousePos.x, (float)mousePos.y);

    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {

            if (m_backButton.getGlobalBounds().contains(mousePosF)) {
                return true;
            }

            if (!m_isPauseMode && m_tabCodeText.getGlobalBounds().contains(mousePosF)) {
                m_currentTab = TAB_CODE;
                m_feedbackText.setString("");
            }
            if (m_tabVolumeText.getGlobalBounds().contains(mousePosF)) {
                m_currentTab = TAB_VOLUME;
            }

            if (m_currentTab == TAB_VOLUME) {
                sf::FloatRect musicHitbox = m_musicTrack.getGlobalBounds();
                musicHitbox.size.y += 20.f;
                musicHitbox.position.y -= 10.f;

                if (musicHitbox.contains(mousePosF)) {
                    m_isDraggingMusic = true;
                    updateSliderValue(mousePosF.x, s_musicVolume, m_musicTrack);
                }

                sf::FloatRect sfxHitbox = m_sfxTrack.getGlobalBounds();
                sfxHitbox.size.y += 20.f;
                sfxHitbox.position.y -= 10.f;

                if (sfxHitbox.contains(mousePosF)) {
                    m_isDraggingSfx = true;
                    updateSliderValue(mousePosF.x, s_sfxVolume, m_sfxTrack);
                }
            }

            if (m_currentTab == TAB_CODE) {
                if (m_validateBtnText.getGlobalBounds().contains(mousePosF)) {
                    if (m_inputString == "speedy") {
                        speedy = !speedy;
                        m_feedbackText.setString(speedy ? "Vitesse activee !" : "Vitesse desactivee");
                        m_feedbackText.setFillColor(sf::Color::Green);
                    }
                    else if (m_inputString == "slow") {
                        slow = !slow;
                        m_feedbackText.setString(slow ? "Ralenti active !" : "Ralenti desactive");
                        m_feedbackText.setFillColor(sf::Color::Green);
                    }
                    else if (m_inputString == "test") {
                        invincible = !invincible;
                        m_feedbackText.setString(invincible ? "Mode testeur actif !" : "Mode testeur inactif");
                        m_feedbackText.setFillColor(sf::Color::Green);
                    }
                    else if (m_inputString == "rainbow") {
                        rainbow = !rainbow;
                        m_feedbackText.setString(rainbow ? "Rainbow active !" : "Rainbow desactive");
                        m_feedbackText.setFillColor(sf::Color::Green);
                    }
                    else if (m_inputString == "skin") {
                        specialSkin = !specialSkin;
                        m_feedbackText.setString(specialSkin ? "Skin Special active !" : "Skin par defaut");
                        m_feedbackText.setFillColor(sf::Color::Cyan);
                    }
                    else {
                        m_feedbackText.setString("Code invalide");
                        m_feedbackText.setFillColor(sf::Color::Red);
                    }
                }
            }
        }
    }

    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            m_isDraggingMusic = false;
            m_isDraggingSfx = false;
        }
    }

    if (event.is<sf::Event::MouseMoved>()) {
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
    else if (m_currentTab == TAB_CODE) {
        window.draw(m_inputBox);
        window.draw(m_inputText);
        window.draw(m_validateBtnText);
        window.draw(m_feedbackText);
    }
}