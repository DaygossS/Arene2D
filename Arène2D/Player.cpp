#include "Player.hpp"

Player::Player(float startX, float startY) {
	m_shape.setSize({ 8.f, 8.f });
	m_shape.setPosition({startX, startY});
	m_shape.setFillColor(sf::Color(0, 255, 206));
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

	m_shape.move(m_speed * m_velocity * deltaTime);

}

void Player::draw(sf::RenderWindow& window) {

	window.draw(m_shape);

}