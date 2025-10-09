#pragma once
#include "cell.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

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

