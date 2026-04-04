#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Configuration;

class Flash : public sf::Drawable
{
	sf::RectangleShape rect;
	sf::Color color;
	Configuration* config;
	std::uint8_t alpha;
public:
	Flash(Configuration* config, sf::Color color);
	void updateAnim(float gameOverTimer);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};