#pragma once
#include "cell.hpp"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Configuration;

class CollisionManager;

class Obstacle : public sf::Drawable
{
	std::vector<Cell> coords;
	sf::VertexArray vertices;
	Configuration* config;
	CollisionManager* collision;
	sf::Shader* shader;
	float time;

	void updateVertices(float creationTime);
public:
	Obstacle(Configuration* config, CollisionManager* collision, sf::Shader* shader);
	void updateShader(float currentTime);
	void generateNewPosition(float creationTime);
	void restoreDefaultValues();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

