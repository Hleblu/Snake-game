#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include "collisionManager.h"
#include "cell.h"
#include "randomGenerator.h"

class Obstacle : public sf::Drawable
{
	std::vector<Cell> coords;
	sf::VertexArray vertices;
	Configuration* config = Configuration::getInstance();
	CollisionManager* collisionManager = CollisionManager::getInstance();

	void updateVertexArray();
public:
	Obstacle();
	void generateNewPosition();
	void restoreDefaultValues();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

