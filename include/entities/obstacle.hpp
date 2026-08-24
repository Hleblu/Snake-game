#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Configuration;

class CollisionManager;

class Obstacle : public sf::Drawable
{
	std::vector<sf::Vector2i> coords;
	sf::VertexArray vertices;
	Configuration& config;
	CollisionManager& collision;
	sf::Shader* shader;
	float shaderTime;

	void updateVertices(float creationTime);
public:
	Obstacle(Configuration& config, CollisionManager& collision, sf::Shader* shader);
	void updateShader(float currentTime);
	void generateNewPosition(float creationTime);
	void restoreDefaultValues();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

