
bool IAisCollidingWithMap(const sf::FloatRect& bounds, const std::vector<int>& collisions) {
    sf::Vector2f points[4] = {
        { bounds.position.x, bounds.position.y },
        { bounds.position.x + bounds.size.x, bounds.position.y },
        { bounds.position.x, bounds.position.y + bounds.size.y },
        { bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y }
    };

    for (int i = 0; i < 4; i++) {
        int gridX = (int)(points[i].x / TILE_SIZE);
        int gridY = (int)(points[i].y / TILE_SIZE);

        if (gridX >= 0 && gridX < MAP_WIDTH && gridY >= 0 && gridY < MAP_HEIGHT) {
            int index = gridY * MAP_WIDTH + gridX;
            if (collisions[index] == 1) return true;
        }
        else {
            return true;
        }
    }
    return false;
}



bool IAisCollidingWithTrail(const sf::FloatRect& bounds, const sf::Vector2f& velocity, const sf::Image& mask) {


    float margin = 2.f;


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


    sf::Vector2u maskSize = mask.getSize();
    for (const auto& point : checkPoints) {
        unsigned int x = (unsigned int)point.x;
        unsigned int y = (unsigned int)point.y;

        if (x < maskSize.x && y < maskSize.y) {
            if (mask.getPixel({ x, y }) == sf::Color::White || mask.getPixel({ x, y }) == sf::Color::Red) {
                return true;
            }
        }
    }
    return false;
}


#include "Bike.h"

void Npc::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}

Npc::Npc(float startX, float startY, const PlayerTextures& texture) : textures(texture), sprite(texture.right) {

	sf::FloatRect bounds = sprite.getLocalBounds();
	float originX = bounds.size.x / 2.f;
	float originY = bounds.size.y / 2.f;
	sprite.setOrigin({ originX, originY });
	sprite.setScale({ 0.5f, 0.5f });
	sprite.setPosition({ startX, startY });
	m_speed = 150.f;
	m_velocity = { -1.f, 0.f };
}



void Npc::reset(float startX, float startY) {
    sprite.setPosition({ startX, startY });
    m_velocity = { 0.f, 0.f };


    sprite.setTexture(textures.right, true);
    updateOrigin();
}

void Npc::updateOrigin() {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
}

//#include "Player.hpp"
//
//Player::Player(float startX, float startY) {
//	m_shape.setSize({ 8.f, 8.f });
//	m_shape.setPosition({ startX, startY });
//	m_shape.setFillColor(sf::Color(100, 100, 100));
//	m_speed = 150.f;
//	m_velocity = { 0.f, 0.f };
//}
//
//void Player::handleInput() {
//
//	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) && (m_velocity.y != 1.f)) {
//		m_velocity.y = -1.f;
//		m_velocity.x = 0.f;
//	}
//	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) && (m_velocity.y != -1.f)) {
//		m_velocity.y = 1.f;
//		m_velocity.x = 0.f;
//	}
//	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) && (m_velocity.x != 1.f)) {
//		m_velocity.x = -1.f;
//		m_velocity.y = 0.f;
//	}
//	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) && (m_velocity.x != -1.f)) {
//		m_velocity.x = 1.f;
//		m_velocity.y = 0.f;
//	}
//}
//
//void Player::update(float deltaTime) {
//
//	m_shape.move(m_speed * m_velocity * deltaTime);
//
//}
//
//void Player::draw(sf::RenderWindow& window) {
//
//	window.draw(m_shape);
//
//}


