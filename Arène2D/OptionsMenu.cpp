#include "OptionsMenu.hpp"
#include <iostream>
#include <cmath> // pour std::round


static sf::Color Gris = sf::Color(100, 100, 100);

OptionsMenu::OptionsMenu()
    : m_currentTab(TAB_VOLUME),
    m_musicVolume(0.5f), m_sfxVolume(0.5f),
    m_isDraggingMusic(false), m_isDraggingSfx(false),
    // --- CORRECTION SFML 3.0 ---
    // On doit lier TOUS les textes à la police ici
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

    // Utilisation de openFromFile comme demandé
    if (!m_font.openFromFile("../Assets/Fonts/arial.ttf")) return false;

    // --- 1. CONFIGURATION DES ONGLETS (HAUT) ---
    m_tabVolumeText.setFont(m_font);
    m_tabVolumeText.setString("VOLUME");
    m_tabVolumeText.setCharacterSize(30);
    m_tabVolumeText.setPosition({ width * 0.3f, 50.f });

    m_tabCodeText.setFont(m_font);
    m_tabCodeText.setString("CODE");
    m_tabCodeText.setCharacterSize(30);
    m_tabCodeText.setPosition({ width * 0.7f, 50.f });

    // --- 2. CONFIGURATION SLIDERS (VOLUME) ---
    // Musique
    m_musicTrack.setSize({ 300.f, 5.f });
    m_musicTrack.setFillColor(sf::Color(100, 100, 100));
    m_musicTrack.setPosition({ (width - 300.f) / 2.f, 200.f }); // Centré

    m_musicHandle.setRadius(10.f);
    m_musicHandle.setOrigin({ 10.f, 10.f }); // Origine au centre du rond
    m_musicHandle.setFillColor(sf::Color::White);
    alignSlider(m_musicVolume, m_musicTrack, m_musicHandle);

    m_musicLabel.setFont(m_font);
    m_musicLabel.setCharacterSize(24);
    m_musicLabel.setPosition({ m_musicTrack.getPosition().x, 160.f });

    // SFX
    m_sfxTrack.setSize({ 300.f, 5.f });
    m_sfxTrack.setFillColor(sf::Color(100, 100, 100));
    m_sfxTrack.setPosition({ (width - 300.f) / 2.f, 350.f });

    m_sfxHandle.setRadius(10.f);
    m_sfxHandle.setOrigin({ 10.f, 10.f });
    m_sfxHandle.setFillColor(sf::Color::White);
    alignSlider(m_sfxVolume, m_sfxTrack, m_sfxHandle);

    m_sfxLabel.setFont(m_font);
    m_sfxLabel.setCharacterSize(24);
    m_sfxLabel.setPosition({ m_sfxTrack.getPosition().x, 310.f });

    // --- 3. CONFIGURATION CODE ---
    m_inputBox.setSize({ 300.f, 40.f });
    m_inputBox.setFillColor(sf::Color(50, 50, 50));
    m_inputBox.setOutlineColor(sf::Color::White);
    m_inputBox.setOutlineThickness(1.f);
    m_inputBox.setPosition({ (width - 400.f) / 2.f, 250.f }); // Un peu à gauche pour laisser place au bouton

    m_inputText.setFont(m_font);
    m_inputText.setCharacterSize(24);
    m_inputText.setFillColor(sf::Color::White);
    m_inputText.setPosition({ m_inputBox.getPosition().x + 5.f, m_inputBox.getPosition().y + 5.f });

    // Bouton Valider
    m_validateBtnShape.setSize({ 90.f, 40.f });
    m_validateBtnShape.setFillColor(Gris);
    m_validateBtnShape.setPosition({ m_inputBox.getPosition().x + 310.f, 250.f });

    m_validateBtnText.setFont(m_font);
    m_validateBtnText.setString("OK");
    m_validateBtnText.setCharacterSize(20);
    m_validateBtnText.setPosition({ m_validateBtnShape.getPosition().x + 30.f, m_validateBtnShape.getPosition().y + 8.f });

    m_feedbackText.setFont(m_font);
    m_feedbackText.setCharacterSize(20);
    m_feedbackText.setFillColor(sf::Color::Green);
    m_feedbackText.setPosition({ m_inputBox.getPosition().x, 300.f });

    // --- 4. BOUTON RETOUR ---
    m_backButton.setFont(m_font);
    m_backButton.setString("RETOUR");
    m_backButton.setCharacterSize(30);
    m_backButton.setFillColor(sf::Color::Red);
    // Centré en bas
    sf::FloatRect textRect = m_backButton.getLocalBounds();
    m_backButton.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
    m_backButton.setPosition({ width / 2.f, height - 80.f });

    update(); // Mise à jour initiale des textes
    return true;
}

void OptionsMenu::alignSlider(float volume, sf::RectangleShape& track, sf::CircleShape& handle) {
    float x = track.getPosition().x + (volume * track.getSize().x);
    handle.setPosition({ x, track.getPosition().y + 2.5f }); // +2.5 pour centrer verticalement sur la barre de 5px
}

void OptionsMenu::updateSliderValue(float mouseX, float& volume, const sf::RectangleShape& track) {
    float relativeX = mouseX - track.getPosition().x;
    volume = relativeX / track.getSize().x;
    
    // Clamp entre 0 et 1
    if (volume < 0.f) volume = 0.f;
    if (volume > 1.f) volume = 1.f;
}

void OptionsMenu::update() {
    // Mise à jour des textes des sliders
    m_musicLabel.setString("Musique : " + std::to_string((int)(m_musicVolume * 100)) + "%");
    m_sfxLabel.setString("SFX : " + std::to_string((int)(m_sfxVolume * 100)) + "%");

    // Mise à jour visuelle des onglets (Couleur)
    if (m_currentTab == TAB_VOLUME) {
        m_tabVolumeText.setFillColor(sf::Color::Blue);
        m_tabCodeText.setFillColor(Gris);
        
    } else {
        m_tabVolumeText.setFillColor(Gris);
        m_tabCodeText.setFillColor(sf::Color::Blue);
        
    }
}

bool OptionsMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    // --- GESTION DES CLICS ---
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            
            // 1. Changement d'onglet
            if (m_tabVolumeText.getGlobalBounds().contains(mousePosF)) {
                m_currentTab = TAB_VOLUME;
                m_feedbackText.setString(""); // Effacer feedback si on change
            }
            else if (m_tabCodeText.getGlobalBounds().contains(mousePosF)) {
                m_currentTab = TAB_CODE;
            }

            // 2. Bouton Retour
            if (m_backButton.getGlobalBounds().contains(mousePosF)) {
                return true; // Signal qu'on veut quitter
            }

            // 3. Logique Onglet VOLUME
            if (m_currentTab == TAB_VOLUME) {
                // Vérifier clic sur les barres pour commencer le drag
                sf::FloatRect musicArea = m_musicTrack.getGlobalBounds();
                musicArea.size.y += 20; // Zone plus large pour cliquer facile
                musicArea.position.y -= 10;

                sf::FloatRect sfxArea = m_sfxTrack.getGlobalBounds();
                sfxArea.size.y += 20;
                sfxArea.position.y -= 10;

                if (musicArea.contains(mousePosF)) m_isDraggingMusic = true;
                if (sfxArea.contains(mousePosF)) m_isDraggingSfx = true;
            }

            // 4. Logique Onglet CODE
            if (m_currentTab == TAB_CODE) {
                if (m_validateBtnShape.getGlobalBounds().contains(mousePosF)) {
                    // Simulation de validation
                    if (m_inputString == "test") {
                        m_feedbackText.setString("Code Accepte : Mode Dieu !");
                        m_feedbackText.setFillColor(sf::Color::Green);
                    } else {
                        m_feedbackText.setString("Code Invalide.");
                        m_feedbackText.setFillColor(sf::Color::Red);
                    }
                }
            }
        }
    }

    // --- GESTION RELACHEMENT CLIC (Fin du drag) ---
    if (const auto* mouseRel = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseRel->button == sf::Mouse::Button::Left) {
            m_isDraggingMusic = false;
            m_isDraggingSfx = false;
        }
    }

    // --- GESTION MOUVEMENT SOURIS (Drag Sliders) ---
    if (event.is<sf::Event::MouseMoved>()) {
        // Survol bouton retour
        if (m_backButton.getGlobalBounds().contains(mousePosF)) m_backButton.setFillColor(sf::Color::Cyan);
        else m_backButton.setFillColor(sf::Color::White);

        if (m_currentTab == TAB_VOLUME) {
            if (m_isDraggingMusic) {
                updateSliderValue(mousePosF.x, m_musicVolume, m_musicTrack);
                alignSlider(m_musicVolume, m_musicTrack, m_musicHandle);
            }
            if (m_isDraggingSfx) {
                updateSliderValue(mousePosF.x, m_sfxVolume, m_sfxTrack);
                alignSlider(m_sfxVolume, m_sfxTrack, m_sfxHandle);
            }
        }
    }

    // --- GESTION SAISIE TEXTE (Onglet Code uniquement) ---
    if (m_currentTab == TAB_CODE) {
        if (const auto* textEvt = event.getIf<sf::Event::TextEntered>()) {
            // Unicode 8 = Backspace
            if (textEvt->unicode == 8) {
                if (!m_inputString.empty()) m_inputString.pop_back();
            }
            // Unicode 13 = Enter (Optionnel, si tu veux valider avec Entrée)
            else if (textEvt->unicode == 13) {
                // Code de validation ici aussi si tu veux
            }
            // Caractères imprimables standards (Lettres, Chiffres...)
            else if (textEvt->unicode < 128) {
                m_inputString += static_cast<char>(textEvt->unicode);
            }
            m_inputText.setString(m_inputString);
        }
    }
    
    update(); // Mettre à jour les textes
    return false; // Par défaut, on reste dans le menu
}

void OptionsMenu::draw(sf::RenderWindow& window) {
    // Toujours dessiner les onglets et le bouton retour
    window.draw(m_tabVolumeText);
    window.draw(m_tabCodeText);
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
        window.draw(m_validateBtnShape);
        window.draw(m_validateBtnText);
        window.draw(m_feedbackText);
    }
}