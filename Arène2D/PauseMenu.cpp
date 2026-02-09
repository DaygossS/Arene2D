#include "PauseMenu.hpp"

// Ta couleur personnalisée
static sf::Color Bleu = sf::Color(0, 50, 100);

PauseMenu::PauseMenu(float width, float height)
    : m_width(width), m_height(height),
    m_state(PAUSE_MAIN), m_selectedItemIndex(-1),
    m_title(m_font) // Initialisation du titre avec la font (requis SFML 3)
{
}

bool PauseMenu::init(const std::string& fontPath) {
    // 1. Chargement de la police
    if (!m_font.openFromFile(fontPath)) return false;

    // 2. Fond semi-transparent (Noir avec 200/255 d'opacité)
    m_background.setSize({ m_width, m_height });
    m_background.setFillColor(sf::Color(0, 0, 0, 200));

    // 3. Configuration du Titre "PAUSE"
    // (Je l'ai déplacé ici pour ne pas le recréer à chaque frame, c'est plus performant)
    m_title.setString("PAUSE");
    m_title.setCharacterSize(80);
    m_title.setFillColor(sf::Color::White);
    m_title.setStyle(sf::Text::Bold);
    sf::FloatRect tRect = m_title.getLocalBounds();
    m_title.setOrigin({ tRect.size.x / 2.f, tRect.size.y / 2.f });
    m_title.setPosition({ m_width / 2.f, 150.f });

    // 4. Configuration des Boutons
    m_buttons.clear();
    std::string texts[3] = { "REPRENDRE", "OPTIONS", "RETOUR MENU" };

    for (int i = 0; i < 3; i++) {
        // Note: Ici on crée une copie locale, puis on la pousse dans le vecteur.
        // Comme m_font est membre de la classe, elle reste valide.
        sf::Text text(m_font);
        text.setString(texts[i]);
        text.setCharacterSize(50);
        text.setFillColor(Bleu);
        text.setOutlineColor(Bleu);
        text.setOutlineThickness(2);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });

        // Positionnement selon ta formule
        text.setPosition({ m_width / 2.f, (m_height / 2.f) - 50.f + (i * 100.f) });

        m_buttons.push_back(text);
    }

    // 5. Init du sous-menu Options
    if (!m_optionsMenu.init(fontPath, m_width, m_height)) return false;

    // Important : On dit au menu option qu'il est en mode Pause (cache l'onglet Code)
    m_optionsMenu.setIsPauseMode(true);

    return true;
}

void PauseMenu::update() {
    if (m_state == PAUSE_OPTIONS) {
        m_optionsMenu.update();
    }
}

int PauseMenu::handleInput(sf::RenderWindow& window, const sf::Event& event) {
    // Si on est dans les options, on délègue tout au menu Options
    if (m_state == PAUSE_OPTIONS) {
        // Si handleEvent renvoie true (bouton retour cliqué), on revient au menu principal de pause
        if (m_optionsMenu.handleEvent(event, window)) {
            m_state = PAUSE_MAIN;
        }
        return 0;
    }

    // --- Gestion du Menu Pause Principal ---

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    // Gestion du survol (Hover)
    m_selectedItemIndex = -1;
    for (size_t i = 0; i < m_buttons.size(); i++) {
        if (m_buttons[i].getGlobalBounds().contains(mousePosF)) {
            m_buttons[i].setFillColor(sf::Color::White); // Devient blanc au survol
            m_selectedItemIndex = static_cast<int>(i);
        }
        else {
            m_buttons[i].setFillColor(Bleu); // Reste bleu sinon
        }
    }

    // Gestion du Clic
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

void PauseMenu::draw(sf::RenderWindow& window) {
    // On dessine toujours le fond sombre en premier
    window.draw(m_background);

    if (m_state == PAUSE_OPTIONS) {
        m_optionsMenu.draw(window);
    }
    else {
        // Affichage du titre
        window.draw(m_title);

        // Affichage des boutons
        for (const auto& btn : m_buttons) {
            window.draw(btn);
        }
    }
}