#include "Player.hpp"

Player::Player(float startX, float startY, const sf::Texture& texture) : sprite(texture) {

	sf::FloatRect bounds = sprite.getLocalBounds();
	float originX = bounds.size.x / 2.f;
	float originY = bounds.size.y / 2.f;
	sprite.setTexture(texture);
	sprite.setOrigin({originX, originY});
	sprite.setScale({0.5f, 0.5f});
	sprite.setPosition({ startX, startY });
	m_speed = 150.f;
	m_velocity = { 0.f, 0.f };
}

void Player::handleInput() {

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) && (m_velocity.y != 1.f)) {
		m_velocity.y = -1.f;
		m_velocity.x = 0.f;
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) && (m_velocity.y != -1.f)) {
		m_velocity.y = 1.f;
		m_velocity.x = 0.f;
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) && (m_velocity.x != 1.f)) {
		m_velocity.x = -1.f;
		m_velocity.y = 0.f;
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) && (m_velocity.x != -1.f)) {
		m_velocity.x = 1.f;
		m_velocity.y = 0.f;
	}
}

void Player::update(float deltaTime){

	sprite.move(m_speed * m_velocity * deltaTime);

}

void Player::draw(sf::RenderWindow& window) {

	window.draw(sprite);

}