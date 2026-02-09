#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class OptionsMenu {
public:
    OptionsMenu();

    // Initialisation (taille fenêtre pour centrer les éléments)
    bool init(const std::string& fontPath, float width, float height);

    // Gère les événements (Souris + Clavier)
    // Retourne true si on doit quitter le menu (bouton Retour cliqué)
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window);

    // Met à jour les pourcentages affichés
    void update();

    // Dessine le menu selon l'onglet actif
    void draw(sf::RenderWindow& window);

    // Pour récupérer les valeurs plus tard
    float getMusicVolume() const { return m_musicVolume; }
    float getSfxVolume() const { return m_sfxVolume; }

private:
    sf::Font m_font;
    float m_windowWidth;
    float m_windowHeight;

    // --- GESTION DES ONGLETS ---
    enum Tab { TAB_VOLUME, TAB_CODE };
    Tab m_currentTab;

    sf::Text m_tabVolumeText;
    sf::Text m_tabCodeText;
    

    // --- ÉLÉMENTS : VOLUME ---
    float m_musicVolume; // 0.0 à 1.0
    float m_sfxVolume;   // 0.0 à 1.0
    bool m_isDraggingMusic;
    bool m_isDraggingSfx;

    sf::RectangleShape m_musicTrack;
    sf::CircleShape m_musicHandle;
    sf::Text m_musicLabel;

    sf::RectangleShape m_sfxTrack;
    sf::CircleShape m_sfxHandle;
    sf::Text m_sfxLabel;

    // --- ÉLÉMENTS : CODE ---
    std::string m_inputString;
    sf::Text m_inputText;
    sf::RectangleShape m_inputBox;
    
    sf::Text m_validateBtnText;
    sf::RectangleShape m_validateBtnShape;
    sf::Text m_feedbackText; // Affiche "Code Valide !" ou "Erreur"

    // --- COMMUN ---
    sf::Text m_backButton;

    // Fonctions utilitaires internes
    void alignSlider(float volume, sf::RectangleShape& track, sf::CircleShape& handle);
    void updateSliderValue(float mouseX, float& volume, const sf::RectangleShape& track);
};