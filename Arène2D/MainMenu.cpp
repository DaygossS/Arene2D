#include "MainMenu.hpp"
#include <iostream>

static sf::Color Bleu = sf::Color(0, 50, 100);

MainMenu::MainMenu(float width, float height)
    : m_width(width), m_height(height), m_state(STATE_MAIN),
    m_gridSprite(m_gridTexture),
    m_carSprite(m_carTexture),
    m_animationTimer(0.f),
    m_currentFrame(0),
    m_selectedItemIndex(-1)
{
}

bool MainMenu::init(const std::string& fontPath) {
    if (!m_font.openFromFile("../Assets/Fonts/arial.ttf")) return false;

    sf::RenderTexture rt;
    int gridSize = 40;

    if (!rt.resize({ (unsigned int)gridSize, (unsigned int)gridSize })) return false;

    rt.clear(sf::Color::Black);

    sf::RectangleShape cell({ (float)gridSize, (float)gridSize });
    cell.setFillColor(sf::Color::Transparent);
    cell.setOutlineColor(Bleu);
    cell.setOutlineThickness(-1.f);

    rt.draw(cell);
    rt.display();

    m_gridTexture = rt.getTexture();
    m_gridTexture.setRepeated(true);
    m_gridSprite.setTexture(m_gridTexture);
    m_gridSprite.setTextureRect(sf::IntRect({ 0, 0 }, { (int)m_width, (int)m_height }));

    // --- MENU PRINCIPAL ---
    m_mainOptions.clear();
    std::string mainTexts[3] = { "JOUER", "OPTIONS", "QUITTER" };

    float totalHeightMain = 3 * 100.f;
    float startYMain = (m_height / 2.f) - (totalHeightMain / 2.f) + 25.f;

    for (int i = 0; i < 3; i++) {
        sf::Text text(m_font);
        text.setString(mainTexts[i]);
        text.setCharacterSize(50);
        text.setFillColor(Bleu);
        text.setOutlineColor(Bleu);
        text.setOutlineThickness(2);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
        text.setPosition({ m_width * 0.25f, startYMain + (i * 100.f) });

        m_mainOptions.push_back(text);
    }

    // --- MENU NIVEAUX ---
    m_levelOptions.clear();
    float totalHeightLevels = 5 * 80.f;
    float startYLevels = (m_height / 2.f) - (totalHeightLevels / 2.f) + 20.f;

    for (int i = 0; i < 5; i++) {
        sf::Text text(m_font);
        text.setString("NIVEAU " + std::to_string(i + 1));
        text.setCharacterSize(40);
        text.setFillColor(Bleu);
        text.setOutlineColor(Bleu);
        text.setOutlineThickness(2);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin({ textRect.size.x / 2.f, textRect.size.y / 2.f });
        text.setPosition({ m_width * 0.25f, startYLevels + (i * 80.f) });

        m_levelOptions.push_back(text);
    }

    if (!m_carTexture.loadFromFile("../Assets/MenuVisu.png")) {
        std::cout << "Erreur : Impossible de charger MenuVisu.png" << std::endl;
    }
    else {
        const int CAR_COLS = 7;
        const int CAR_ROWS = 7;

        int frameW = m_carTexture.getSize().x / CAR_COLS;
        int frameH = m_carTexture.getSize().y / CAR_ROWS;

        m_carRect = sf::IntRect({ 0, 0 }, { frameW, frameH });

        m_carSprite.setTexture(m_carTexture);
        m_carSprite.setTextureRect(m_carRect);
        m_carSprite.setOrigin({ frameW / 2.f, frameH / 2.f });
        m_carSprite.setPosition({ m_width * 0.70f, m_height / 2.f });
        m_carSprite.setScale({ 3.5f, 3.5f });
    }

    if (!m_optionsMenu.init("../Assets/Fonts/arial.ttf", m_width, m_height)) return false;

    return true;
}

void MainMenu::update(float deltaTime) {
    if (m_state == STATE_OPTIONS) {
        m_optionsMenu.update();
        return;
    }

    const float TIME_PER_FRAME = 0.08f;
    const int TOTAL_FRAMES = 48;
    const int CAR_COLS = 7;

    m_animationTimer += deltaTime;

    if (m_animationTimer >= TIME_PER_FRAME) {
        m_animationTimer = 0.f;
        m_currentFrame++;
        if (m_currentFrame >= TOTAL_FRAMES) m_currentFrame = 0;

        int col = m_currentFrame % CAR_COLS;
        int row = m_currentFrame / CAR_COLS;

        m_carRect.position.x = col * m_carRect.size.x;
        m_carRect.position.y = row * m_carRect.size.y;

        m_carSprite.setTextureRect(m_carRect);
    }
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(m_gridSprite);

    if (m_state == STATE_OPTIONS) {
        m_optionsMenu.draw(window);
    }
    else if (m_state == STATE_LEVELS) {
        window.draw(m_carSprite);
        for (const auto& text : m_levelOptions) {
            window.draw(text);
        }
    }
    else {
        window.draw(m_carSprite);
        for (const auto& text : m_mainOptions) {
            window.draw(text);
        }
    }
}

int MainMenu::handleInput(sf::RenderWindow& window, const sf::Event& event) {
    if (m_state == STATE_OPTIONS) {
        if (m_optionsMenu.handleEvent(event, window)) {
            m_state = STATE_MAIN;
        }
        return 0;
    }

    std::vector<sf::Text>* currentMenu = (m_state == STATE_LEVELS) ? &m_levelOptions : &m_mainOptions;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    m_selectedItemIndex = -1;
    for (size_t i = 0; i < currentMenu->size(); i++) {
        if ((*currentMenu)[i].getGlobalBounds().contains(mousePosF)) {
            (*currentMenu)[i].setFillColor(sf::Color::White);
            m_selectedItemIndex = static_cast<int>(i);
        }
        else {
            (*currentMenu)[i].setFillColor(Bleu);
        }
    }

    if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left && m_selectedItemIndex != -1) {

            if (m_state == STATE_MAIN) {
                if (m_selectedItemIndex == 0) {
                    m_state = STATE_LEVELS;
                    return 0;
                }
                if (m_selectedItemIndex == 1) {
                    m_state = STATE_OPTIONS;
                    return 0;
                }
                if (m_selectedItemIndex == 2) return 3;
            }
            else if (m_state == STATE_LEVELS) {
                m_selectedLevelFile = "../Assets/Levels/niveau" + std::to_string(m_selectedItemIndex + 1) + ".txt";
                return 1;
            }
        }
    }

    // Gestion du retour au menu principal avec Echap si on est dans les niveaux
    if (m_state == STATE_LEVELS) {
        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->scancode == sf::Keyboard::Scancode::Escape) {
                m_state = STATE_MAIN;
            }
        }
    }

    return 0;
}

std::string MainMenu::getSelectedLevelFile() const {
    return m_selectedLevelFile;
}