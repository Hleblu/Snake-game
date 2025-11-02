#pragma once
#include "cell.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Obstacle : public sf::Drawable
{
	std::vector<Cell> coords;
	sf::VertexArray vertices;

	void updateVertices(float creationTime);
public:
	Obstacle();
	void generateNewPosition(float creationTime);
	void restoreDefaultValues();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

