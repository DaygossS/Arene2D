#include "MainMenu.hpp"

MainMenu::MainMenu(float width, float height)
    : m_width(width), m_height(height), m_showOptions(false) // Par défaut, on n'affiche pas les options
{
}

bool MainMenu::init(const std::string& fontPath) {
    if (!m_font.openFromFile("../Assets/Fonts/arial.ttf")) return false;

    // 1. Init des boutons principaux
    std::string texts[3] = { "JOUER", "OPTIONS", "QUITTER" };
    for (int i = 0; i < 3; i++) {
        sf::Text text(m_font);
        text.setString(texts[i]);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Cyan);
        text.setOutlineThickness(2);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
        text.setPosition({ m_width / 2.f, 300.f + (i * 100.f) }); // Centré horizontalement

        m_options.push_back(text);
    }

    // 2. Init du sous-menu Options
    // On passe les mêmes paramètres
    if (!m_optionsMenu.init(fontPath, m_width, m_height)) return false;

    return true;
}

void MainMenu::draw(sf::RenderWindow& window) {
    if (m_showOptions) {
        // Si on est dans les options, on laisse l'autre classe dessiner
        m_optionsMenu.draw(window);
    }
    else {
        // Sinon, on dessine nos boutons
        for (const auto& text : m_options) {
            window.draw(text);
        }
    }
}

int MainMenu::handleInput(sf::RenderWindow& window, const sf::Event& event) {
    // CAS 1 : On est dans le sous-menu OPTIONS
    if (m_showOptions) {
        // On délègue tout à OptionsMenu
        // Si handleEvent renvoie true (le bouton Retour a été cliqué)
        if (m_optionsMenu.handleEvent(event, window)) {
            m_showOptions = false; // On revient au menu principal
        }
        return 0; // On reste dans l'état global "MENU" du main.cpp
    }

    // CAS 2 : On est dans le MENU PRINCIPAL
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    // Survol
    m_selectedItemIndex = -1;
    for (size_t i = 0; i < m_options.size(); i++) {
        if (m_options[i].getGlobalBounds().contains(mousePosF)) {
            m_options[i].setFillColor(sf::Color::Cyan);
            m_selectedItemIndex = i;
        }
        else {
            m_options[i].setFillColor(sf::Color::White);
        }
    }

    // Clic
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            if (m_selectedItemIndex == 0) return 1; // JOUER -> Change l'état du Main en GAME
            if (m_selectedItemIndex == 1) {
                m_showOptions = true; // OPTIONS -> On reste dans MENU, mais on affiche le sous-menu
                return 0;
            }
            if (m_selectedItemIndex == 2) return 3; // QUITTER -> Ferme la fenêtre
        }
    }

    return 0;
}