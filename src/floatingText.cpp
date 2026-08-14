#include "configuration.hpp"
#include "floatingText.hpp"
#include "randomGenerator.hpp"
#include <algorithm>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

FloatingText::FloatingText(Configuration& config, sf::Shader* shader, sf::Font& font)
	: config(config),
	shader(shader),
	label(font),
	currentAge(1.f),
	lifeTime(0.f)
{
	initAppearence();
}

void FloatingText::initAppearence()
{
	label.setFillColor(config.textBaseColor);
	label.setOutlineColor(config.textOutlineColor);
	label.setOutlineThickness(config.getCellSize() * 0.075f);
}

void FloatingText::updateLabel(const sf::String& str, sf::Vector2f pos, float lifeTime)
{
	this->lifeTime = lifeTime;
	label.setString(str);

	const auto cellSize = config.getCellSize();
	/*
		Eight is the size of tiny5 glyph, which I 
		determined by GCD of those charachter sizes,
		which scaled while preserving clarity.

		Because of it, we are looking for the nearest number,
		which will perfectly divide by eight.
	*/
	unsigned int charachterSize = 8u * (cellSize / 8u);
	label.setCharacterSize(charachterSize);

	const sf::FloatRect bounds = label.getLocalBounds();
	const sf::Vector2f halfSize = bounds.size / 2.f;
	label.setOrigin({ bounds.position + halfSize });

	pos.x += RandomGenerator::getFloat(-cellSize, cellSize);
	pos.y += RandomGenerator::getFloat(-cellSize, cellSize);

	const sf::Vector2f gHalfSize = label.getGlobalBounds().size / 2.f;
	pos.x = std::clamp(pos.x, gHalfSize.x, config.width - gHalfSize.x);
	pos.y = std::clamp(pos.y, gHalfSize.y, config.height - gHalfSize.y);

	label.setPosition(pos);
	currentAge = 0.f;
}

void FloatingText::updateShader(float dt)
{
	currentAge += dt;
}

void FloatingText::hide()
{
	currentAge = lifeTime;
}

void FloatingText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (currentAge > lifeTime) return;
	if (shader)
	{
		const float normalizedTime = std::min(currentAge / lifeTime, 1.0f);
		shader->setUniform("time", normalizedTime);
		shader->setUniform("texture", sf::Shader::CurrentTexture);
		states.shader = shader;
	}

	target.draw(label, states);
}
