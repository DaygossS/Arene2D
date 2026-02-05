#include "Player.hpp"

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;

Player::Player(float startX, float startY): trailDisplay(trailTexture.getTexture()){
	m_shape.setSize({ 8.f, 8.f });
	m_shape.setPosition({startX, startY});
	m_shape.setFillColor(sf::Color(100, 100 ,100));
	m_speed = 150.f;
	m_velocity = { 0.f, 0.f };
	trailBrush.setFillColor(sf::Color::White);
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
	trailTexture.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) });
	trailMask.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);


}

void Player::draw(sf::RenderWindow& window) {

	window.draw(m_shape);
	trailTexture.clear(sf::Color::Transparent);
	

	trailBrush.setSize({ m_shape.getSize().x - 2, m_shape.getSize().y - 2 });
	trailBrush.setPosition({ m_shape.getPosition().x + 1, m_shape.getPosition().y + 1 });
	trailTexture.draw(trailBrush);
	trailTexture.display();


	sf::Vector2u maskSize = trailMask.getSize();

	int startX = (int)(m_shape.getPosition().x + 1);
	int startY = (int)(m_shape.getPosition().y + 1);
	int endX = startX + (int)(m_shape.getSize().x - 2);
	int endY = startY + (int)(m_shape.getSize().y - 2);


	for (int x = startX; x < endX; x++) {
		for (int y = startY; y < endY; y++) {
			if (x >= 0 && x < (int)maskSize.x && y >= 0 && y < (int)maskSize.y) {

				trailMask.setPixel({ (unsigned int)x, (unsigned int)y }, sf::Color::White);
			}
		}
	}

	window.draw(trailDisplay);

}

void Player::CleartrailTexture(){
	trailTexture.clear(sf::Color::Transparent);
}

void Player::SettrailMask(sf::Image newtrailMask) {
	trailMask = newtrailMask;
};
