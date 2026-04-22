#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Configuration;

class FloatingText : public sf::Drawable
{
	static constexpr float defaultLifeTime = 0.8f;

	sf::Text label;
	sf::Font* font;
	Configuration* config;
	sf::Shader* shader;
	float currentAge;
	float lifeTime;

	void initAppearence();
public:
	FloatingText(Configuration* config, sf::Shader* shader, sf::Font* font);
	void updateLabel(const std::string& str, sf::Vector2f pos, float lifeTime = defaultLifeTime);
	void updateShader(float dt);
	void hide();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};