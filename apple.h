#pragma once
#include "constants.h"
#include "snake.h"
#include <random>

class Apple : public sf::Drawable
{
	Snake* snake;
	sf::RectangleShape rect;
	std::random_device rd;
	std::mt19937 gen;
	float x, y;

	Apple(Snake& snake);
	void genCords();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	friend class Game;
};