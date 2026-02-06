#include "Player.hpp"

Player::Player(float startX, float startY, const PlayerTextures& texture) : textures(texture), sprite(texture.right) {

	sf::FloatRect bounds = sprite.getLocalBounds();
	float originX = bounds.size.x / 2.f;
	float originY = bounds.size.y / 2.f;
	
	sprite.setOrigin({originX, originY});
	sprite.setScale({0.3f, 0.3f});
	sprite.setPosition({ startX, startY });
	m_speed = 150.f;
	m_velocity = { 0.f, 0.f };
}

void Player::reset(float startX,float startY) {

	sprite.setPosition(sf::Vector2f(startX, startY));
	m_velocity = sf::Vector2f(0.f, 0.f);
	sprite.setTexture(textures.right);


}

void Player::handleInput() {

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) && (m_velocity.y != 1.f)) {
		m_velocity.y = -1.f;
		m_velocity.x = 0.f;
		sprite.setTexture(textures.up);
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) && (m_velocity.y != -1.f)) {
		m_velocity.y = 1.f;
		m_velocity.x = 0.f;
		sprite.setTexture(textures.down);
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) && (m_velocity.x != 1.f)) {
		m_velocity.x = -1.f;
		m_velocity.y = 0.f;
		sprite.setTexture(textures.left);
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) && (m_velocity.x != -1.f)) {
		m_velocity.x = 1.f;
		m_velocity.y = 0.f;
		sprite.setTexture(textures.right);
	}
}

void Player::update(float deltaTime){

	sprite.move(m_speed * m_velocity * deltaTime);

}

void Player::draw(sf::RenderWindow& window) {

	window.draw(sprite);

}
sf::FloatRect Player::getBounds() const {
	return sprite.getGlobalBounds();
}