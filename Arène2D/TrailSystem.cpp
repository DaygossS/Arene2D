#include "TrailSystem.hpp"

TrailSystem::TrailSystem(int width, int height, int tileSize, float thickness)
    : m_thickness(thickness),
    // On lie la texture au sprite dès le début pour satisfaire SFML 3
    m_sprite(m_texture.getTexture())
{
    unsigned int w = (unsigned int)(width * tileSize);
    unsigned int h = (unsigned int)(height * tileSize);

    // 1. On redimensionne la texture
    if (!m_texture.resize({ w, h })) {
        std::cerr << "Erreur: Impossible de creer la texture de trainee." << std::endl;
    }

    // 2. CORRECTION CRUCIALE : 
    // On force le sprite à prendre la nouvelle taille de la texture.
    // Le 'true' signifie "resetRect" (réinitialiser le rectangle d'affichage)
    m_sprite.setTexture(m_texture.getTexture(), true);

    m_texture.clear(sf::Color::Transparent);

    // Initialisation Masque (Collision)
    m_mask.resize({ w, h }, sf::Color::Black);

    // Configuration du pinceau
    m_brush.setSize({ m_thickness, m_thickness });
}

void TrailSystem::addTrail(sf::Vector2f position, sf::Color color) {
    float brushX = position.x - (m_thickness / 2.f);
    float brushY = position.y - (m_thickness / 2.f);

    // On applique la couleur demandée (Bleu pour joueur, Rouge pour IA, etc.)
    m_brush.setFillColor(color);
    m_brush.setPosition({ brushX, brushY });

    m_texture.draw(m_brush);
    m_texture.display(); // Valide le dessin

    // Mise à jour du masque de collision (toujours en blanc pour la logique)
    int startX = (int)brushX;
    int startY = (int)brushY;
    int endX = startX + (int)m_thickness;
    int endY = startY + (int)m_thickness;

    sf::Vector2u maskSize = m_mask.getSize();

    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
            if (x >= 0 && x < (int)maskSize.x && y >= 0 && y < (int)maskSize.y) {
                m_mask.setPixel({ (unsigned int)x, (unsigned int)y }, sf::Color::White);
            }
        }
    }
}

bool TrailSystem::checkCollision(const sf::FloatRect& bounds, const sf::Vector2f& velocity) const {
    float margin = 4.f;
    std::vector<sf::Vector2f> checkPoints;

    if (velocity.x > 0) {
        checkPoints.push_back({ bounds.position.x + bounds.size.x, bounds.position.y + margin });
        checkPoints.push_back({ bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y - margin });
    }
    else if (velocity.x < 0) {
        checkPoints.push_back({ bounds.position.x, bounds.position.y + margin });
        checkPoints.push_back({ bounds.position.x, bounds.position.y + bounds.size.y - margin });
    }
    else if (velocity.y > 0) {
        checkPoints.push_back({ bounds.position.x + margin, bounds.position.y + bounds.size.y });
        checkPoints.push_back({ bounds.position.x + bounds.size.x - margin, bounds.position.y + bounds.size.y });
    }
    else if (velocity.y < 0) {
        checkPoints.push_back({ bounds.position.x + margin, bounds.position.y });
        checkPoints.push_back({ bounds.position.x + bounds.size.x - margin, bounds.position.y });
    }

    sf::Vector2u maskSize = m_mask.getSize();
    for (const auto& point : checkPoints) {
        unsigned int x = (unsigned int)point.x;
        unsigned int y = (unsigned int)point.y;

        if (x < maskSize.x && y < maskSize.y) {
            if (m_mask.getPixel({ x, y }) == sf::Color::White) {
                return true;
            }
        }
    }
    return false;
}

void TrailSystem::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}

void TrailSystem::reset() {
    m_texture.clear(sf::Color::Transparent);
    m_mask.resize(m_mask.getSize(), sf::Color::Black);
}