#include "GameOverMenu.hpp"


static sf::Color Bleu = sf::Color(0, 50, 100);
GameOverMenu::GameOverMenu()
    : m_width(0), m_height(0),
    // SFML 3.0 : Obligatoire d'init les textes avec la police
    m_titleText(m_font), m_scoreText(m_font),
    m_btnRestart(m_font), m_btnMenu(m_font)
{
}

bool GameOverMenu::init(const std::string& fontPath, float width, float height) {
    m_width = width;
    m_height = height;

    if (!m_font.openFromFile("../Assets/Fonts/arial.ttf")) return false;


    m_background.setSize({ width, height });
    m_background.setFillColor(sf::Color(0, 0, 0, 200));


    m_titleText.setCharacterSize(60);
    m_titleText.setStyle(sf::Text::Bold);



    m_scoreText.setCharacterSize(30);
    m_scoreText.setFillColor(sf::Color::White);


    // --- BOUTON REJOUER ---
    m_btnRestart.setString("REJOUER");
    m_btnRestart.setCharacterSize(40);
    m_btnRestart.setFillColor(sf::Color::White);
    sf::FloatRect rRect = m_btnRestart.getLocalBounds();
    m_btnRestart.setOrigin({ rRect.size.x / 2.f, rRect.size.y / 2.f });
    m_btnRestart.setPosition({ width / 2.f, height / 2.f + 50.f });

    // --- BOUTON MENU ---
    m_btnMenu.setString("MENU PRINCIPAL");
    m_btnMenu.setCharacterSize(40);
    m_btnMenu.setFillColor(sf::Color::White);
    sf::FloatRect mRect = m_btnMenu.getLocalBounds();
    m_btnMenu.setOrigin({ mRect.size.x / 2.f, mRect.size.y / 2.f });
    m_btnMenu.setPosition({ width / 2.f, height / 2.f + 120.f });

    return true;
}

void GameOverMenu::setState(bool isVictory, int score) {
    if (isVictory) {
        m_titleText.setString("VICTOIRE !");
        m_titleText.setFillColor(sf::Color::Green);
    }
    else {
        m_titleText.setString("DEFAITE...");
        m_titleText.setFillColor(sf::Color::Red);
    }

    // Centrage du titre
    sf::FloatRect tRect = m_titleText.getLocalBounds();
    m_titleText.setOrigin({ tRect.size.x / 2.f, tRect.size.y / 2.f });
    m_titleText.setPosition({ m_width / 2.f, m_height / 2.f - 100.f });

    // Score
    m_scoreText.setString("Score Final : " + std::to_string(score));
    sf::FloatRect sRect = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin({ sRect.size.x / 2.f, sRect.size.y / 2.f });
    m_scoreText.setPosition({ m_width / 2.f, m_height / 2.f - 30.f });
}

GameOverAction GameOverMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    // Survol (Hover effect)
    if (m_btnRestart.getGlobalBounds().contains(mousePosF))
        m_btnRestart.setFillColor(sf::Color::White);
    else
        m_btnRestart.setFillColor(Bleu);

    if (m_btnMenu.getGlobalBounds().contains(mousePosF))
        m_btnMenu.setFillColor(sf::Color::White);
    else
        m_btnMenu.setFillColor(Bleu);

    
    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {

            if (m_btnRestart.getGlobalBounds().contains(mousePosF)) {
                return GameOverAction::Restart;
            }

            if (m_btnMenu.getGlobalBounds().contains(mousePosF)) {
                return GameOverAction::BackToMenu;
            }
        }
    }

    return GameOverAction::None;
}

void GameOverMenu::draw(sf::RenderWindow& window) {
   
    window.draw(m_background);

    window.draw(m_titleText);
    window.draw(m_scoreText);
    window.draw(m_btnRestart);
    window.draw(m_btnMenu);
}