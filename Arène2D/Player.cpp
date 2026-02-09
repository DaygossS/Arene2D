#include "Player.hpp"


Player::Player(float startX, float startY, const PlayerTextures& texture)
    : textures(texture), sprite(texture.right)
{
    
    sprite.setTexture(textures.right, true);

    sprite.setScale({ 0.3f, 0.3f });
    sprite.setPosition({ startX, startY });

    m_speed = 300.f;
    m_velocity = { 0.f, 0.f };

    
    updateOrigin();
}

void Player::reset(float startX, float startY) {
    sprite.setPosition({ startX, startY });
    m_velocity = { 0.f, 0.f };

    
    sprite.setTexture(textures.right, true);
    updateOrigin();
}

void Player::handleInput() {
    bool textureChanged = false;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
        && (m_velocity.y != 1.f)) {
        m_velocity.y = -1.f;
        m_velocity.x = 0.f;
        
        sprite.setTexture(textures.up, true);
        textureChanged = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
        && (m_velocity.y != -1.f)) {
        m_velocity.y = 1.f;
        m_velocity.x = 0.f;
        sprite.setTexture(textures.down, true); 
        textureChanged = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
        && (m_velocity.x != 1.f)) {
        m_velocity.x = -1.f;
        m_velocity.y = 0.f;
        sprite.setTexture(textures.left, true); 
        textureChanged = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
        && (m_velocity.x != -1.f)) {
        m_velocity.x = 1.f;
        m_velocity.y = 0.f;
        sprite.setTexture(textures.right, true); 
        textureChanged = true;
    }

   
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


void Player::updateOrigin() {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}