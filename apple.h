#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include "snake.h"
#include <random>

class Apple : public sf::Drawable
{
protected:
	Snake* snake;
	sf::RectangleShape rect;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<> distX, distY;
	short int x, y;
	Configuration* config = Configuration::getInstance();

public:
	Apple(Snake& snake);
	void generateNewPosition();
	bool isEaten() const;
	void updateData();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};