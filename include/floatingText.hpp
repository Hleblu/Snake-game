#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Configuration;

class FloatingText : public sf::Drawable
{
	sf::Text label;
	sf::Font* font;
	Configuration* config;
	sf::Shader* shader;
	float currentAge;
public:

	FloatingText(Configuration* config, sf::Shader* shader, sf::Font* font);
	void initAppearence();
	void updateLabel(const std::string& str, sf::Vector2f pos);
	void updateShader(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};