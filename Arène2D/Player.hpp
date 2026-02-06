#pragma once
#include "SFML/Graphics.hpp"

struct PlayerTextures {
	const sf::Texture& up;
	const sf::Texture& down;
	const sf::Texture& left;
	const sf::Texture& right;
};

class Player
{
public :

	Player(float startX, float startY, const PlayerTextures& texture);
	sf::FloatRect getBounds() const;
	void reset(float startX, float startY);
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
	PlayerTextures textures;
	void updateOrigin();
};

