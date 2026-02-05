#pragma once

#include "Trail.cpp"
#include "SFML/Graphics.hpp"

class Player
{
public :

	Player(float startX, float startY);


	void handleInput();
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	sf::Vector2f getSize() const { return m_shape.getSize(); }
	sf::Vector2f getVelocity() const { return m_velocity; }
private:
	sf::RectangleShape m_shape;
	sf::Vector2f m_velocity;
	float m_speed;
	//Trail line();
	



};

