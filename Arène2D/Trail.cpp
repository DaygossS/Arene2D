#include "Trail.h"

const int TILE_SIZE = 32;
const int MAP_WIDTH = 50;
const int MAP_HEIGHT = 28;

Trail::Trail(sf::RenderTexture trailTexture) : trailDisplay(trailTexture.getTexture()) {
	trailBrush.setFillColor(sf::Color::White);
}


void Trail::CleartrailTexture() {
	trailTexture.clear(sf::Color::Transparent);
}

void Trail::SettrailMask(sf::Image newtrailMask) {
	trailMask = newtrailMask;
};

void Trail::update(float deltaTime) {
	//trailTexture.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) });
	//trailMask.resize({ (unsigned int)(MAP_WIDTH * TILE_SIZE), (unsigned int)(MAP_HEIGHT * TILE_SIZE) }, sf::Color::Black);

	//trailBrush.setSize({ m_shape.getSize().x - 2, m_shape.getSize().y - 2 });
	//trailBrush.setPosition({ m_shape.getPosition().x + 1, m_shape.getPosition().y + 1 });
	trailTexture.draw(trailBrush);
}

void Trail::draw(sf::RenderWindow& window) {

	trailTexture.display();


	sf::Vector2u maskSize = trailMask.getSize();

	/*int startX = (int)(m_shape.getPosition().x + 1);
	int startY = (int)(m_shape.getPosition().y + 1);
	int endX = startX + (int)(m_shape.getSize().x - 2);
	int endY = startY + (int)(m_shape.getSize().y - 2);*/

	//temporary
	int startX = 1;
	int startY = 1;
	int endX = startX + 10;
	int endY = startY + 10;


	for (int x = startX; x < endX; x++) {
		for (int y = startY; y < endY; y++) {
			if (x >= 0 && x < (int)maskSize.x && y >= 0 && y < (int)maskSize.y) {

				trailMask.setPixel({ (unsigned int)x, (unsigned int)y }, sf::Color::White);
			}
		}
	}


	window.draw(trailDisplay);
}