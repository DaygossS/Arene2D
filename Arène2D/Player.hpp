#pragma once
#include "SFML/Graphics.hpp"

class Player
{
public :

	Player(float startX, float startY);


	void handleInput();
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);


private:
	sf::RectangleShape m_shape;
	sf::Vector2f m_velocity;
	float m_speed;
};

