#pragma once
#include "SFML/Graphics.hpp"

class Player
{
public :

	Player(float startX, float startY, const sf::Texture& texture);


	void handleInput();
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	sf::Vector2f getPosition() const { return sprite.getPosition(); }
	sf::Vector2f getScale() const { return sprite.getScale(); }
	sf::Vector2f getVelocity() const { return m_velocity; }
private:
	sf::Sprite sprite;
	sf::Vector2f m_velocity;
	float m_speed;
};

