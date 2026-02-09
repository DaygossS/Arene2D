#include "PauseMenu.hpp"

static sf::Color Bleu = sf::Color(0, 50, 100);

PauseMenu::PauseMenu(float width, float height)
    : m_width(width), m_height(height), m_state(PAUSE_MAIN), m_selectedItemIndex(-1)
{
}

bool PauseMenu::init(const std::string& fontPath) {
    if (!m_font.openFromFile("../Assets/Fonts/arial.ttf")) return false;

    // Fond semi-transparent (Noir avec 200/255 d'opacité)
    m_background.setSize({ m_width, m_height });
    m_background.setFillColor(sf::Color(0, 0, 0, 200));

    // Boutons
    m_buttons.clear();
    std::string texts[3] = { "REPRENDRE", "OPTIONS", "RETOUR MENU" };

    for (int i = 0; i < 3; i++) {
        sf::Text text(m_font);
        text.setString(texts[i]);
        text.setCharacterSize(50);
        text.setFillColor(Bleu);
        text.setOutlineColor(Bleu);
        text.setOutlineThickness(2);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
        text.setPosition({ m_width / 2.f, (m_height / 2.f) - 50.f + (i * 100.f) });

        m_buttons.push_back(text);
    }

    if (!m_optionsMenu.init(fontPath, m_width, m_height)) return false;

    return true;
}

void PauseMenu::update() {
    if (m_state == PAUSE_OPTIONS) {
        m_optionsMenu.update();
    }
}

void PauseMenu::draw(sf::RenderWindow& window) {
    // On dessine toujours le fond sombre
    window.draw(m_background);

    if (m_state == PAUSE_OPTIONS) {
        m_optionsMenu.draw(window);
    }
    else {
        // Titre "PAUSE" (Optionnel, mais sympa)
        sf::Text title(m_font);
        title.setString("PAUSE");
        title.setCharacterSize(80);
        title.setFillColor(sf::Color::White);
        sf::FloatRect tRect = title.getLocalBounds();
        title.setOrigin({ tRect.size.x / 2.f, tRect.size.y / 2.f });
        title.setPosition({ m_width / 2.f, 150.f });
        window.draw(title);

        for (const auto& btn : m_buttons) {
            window.draw(btn);
        }
    }
}

int PauseMenu::handleInput(sf::RenderWindow& window, const sf::Event& event) {
    if (m_state == PAUSE_OPTIONS) {
        if (m_optionsMenu.handleEvent(event, window)) {
            m_state = PAUSE_MAIN; // Retour des options vers le menu pause
        }
        return 0;
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    m_selectedItemIndex = -1;
    for (size_t i = 0; i < m_buttons.size(); i++) {
        if (m_buttons[i].getGlobalBounds().contains(mousePosF)) {
            m_buttons[i].setFillColor(sf::Color::White);
            m_selectedItemIndex = static_cast<int>(i);
        }
        else {
            m_buttons[i].setFillColor(Bleu);
        }
    }

    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left && m_selectedItemIndex != -1) {
            if (m_selectedItemIndex == 0) return 1; // REPRENDRE
            if (m_selectedItemIndex == 1) {         // OPTIONS
                m_state = PAUSE_OPTIONS;
                return 0;
            }
            if (m_selectedItemIndex == 2) return 2; // RETOUR MENU
        }
    }

    // Raccourci Echap pour reprendre
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scancode::Escape) {
            return 1; // Reprendre
        }
    }

    return 0;
}