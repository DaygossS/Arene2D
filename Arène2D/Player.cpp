#include "Player.hpp"

// CORRECTION 1 : "textures(texture)" (et pas textures(textures))
Player::Player(float startX, float startY, const PlayerTextures& texture)
    : textures(texture), sprite(texture.right)
{
    // CORRECTION 2 : On force la mise à jour de la taille dès le début
    sprite.setTexture(textures.right, true);

    sprite.setScale({ 0.3f, 0.3f });
    sprite.setPosition({ startX, startY });

    m_speed = 150.f;
    m_velocity = { 0.f, 0.f };

    // On calcule le centre correctement au démarrage
    updateOrigin();
}

void Player::reset(float startX, float startY) {
    sprite.setPosition({ startX, startY });
    m_velocity = { 0.f, 0.f };

    // CORRECTION : ", true" pour reset la taille de l'image
    sprite.setTexture(textures.right, true);
    updateOrigin();
}

void Player::handleInput() {
    bool textureChanged = false;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
        && (m_velocity.y != 1.f)) {
        m_velocity.y = -1.f;
        m_velocity.x = 0.f;
        // CORRECTION CRUCIALE : Ajout de ", true" pour adapter la hitbox à la verticale
        sprite.setTexture(textures.up, true);
        textureChanged = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
        && (m_velocity.y != -1.f)) {
        m_velocity.y = 1.f;
        m_velocity.x = 0.f;
        sprite.setTexture(textures.down, true); // Ici aussi
        textureChanged = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
        && (m_velocity.x != 1.f)) {
        m_velocity.x = -1.f;
        m_velocity.y = 0.f;
        sprite.setTexture(textures.left, true); // Ici aussi
        textureChanged = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
        && (m_velocity.x != -1.f)) {
        m_velocity.x = 1.f;
        m_velocity.y = 0.f;
        sprite.setTexture(textures.right, true); // Ici aussi
        textureChanged = true;
    }

    // Si on a changé d'image (ex: Passage de Horizontal à Vertical), on recentre le point de pivot
    if (textureChanged) {
        updateOrigin();
    }
}

void Player::update(float deltaTime) {
    sprite.move(m_speed * m_velocity * deltaTime);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

// Cette fonction doit être présente pour que la rotation se fasse autour du centre
void Player::updateOrigin() {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}