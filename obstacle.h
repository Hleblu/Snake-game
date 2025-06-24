#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "cell.h"

class Obstacle : public sf::Drawable
{
	std::vector<Cell> coords;
	sf::VertexArray vertices;

	void updateVertices();
public:
	Obstacle();
	void generateNewPosition();
	void restoreDefaultValues();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

