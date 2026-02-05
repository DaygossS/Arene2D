#pragma once
#include "SFML/Graphics.hpp"
class Trail
{
public:
	Trail(sf::RenderTexture trailTexture);
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);
	void CleartrailTexture();
	void SettrailMask(sf::Image newtrailMask);

	sf::Image GettrailMask() const { return trailMask; }
private:
	sf::RectangleShape trailBrush;

	sf::RenderTexture trailTexture;
	sf::Sprite trailDisplay;
	sf::Image trailMask;
};

