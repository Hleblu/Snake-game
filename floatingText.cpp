#include "floatingText.hpp"
#include "randomGenerator.hpp"
#include "configuration.hpp"
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

constexpr float LIFE_TIME = 0.8f;

FloatingText::FloatingText(Configuration* config, sf::Shader* shader, sf::Font* font)
	: config(config),
	shader(shader),
	font(font),
	label(*font),
	currentAge(LIFE_TIME)
{}

void FloatingText::initAppearence()
{
	label.setFillColor(config->textBaseColor);
	label.setOutlineColor(config->textOutlineColor);
	label.setOutlineThickness(config->getCellSize() * 0.07f);
}

void FloatingText::updateLabel(const std::string& str, sf::Vector2f pos)
{
	label.setString(str);

	const auto cellSize = config->getCellSize();
	label.setCharacterSize(cellSize);

	const sf::FloatRect bounds = label.getLocalBounds();
	const sf::Vector2f halfSize = bounds.size / 2.f;
	label.setOrigin({
		bounds.position.x + halfSize.x,
		bounds.position.y + halfSize.y
	});

	pos.x += RandomGenerator::getFloat(-cellSize, cellSize);
	pos.y += RandomGenerator::getFloat(-cellSize, cellSize);

	pos.x = std::clamp(pos.x, halfSize.x, config->width - halfSize.x);
	pos.y = std::clamp(pos.y, halfSize.y, config->height - halfSize.y);

	label.setPosition(pos);
	currentAge = 0.f;
}

void FloatingText::updateShader(float dt)
{
	currentAge += dt;
}

void FloatingText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (currentAge > LIFE_TIME) return;
	if (shader)
	{
		const float normalizedTime = std::min(currentAge / LIFE_TIME, 1.0f);
		shader->setUniform("time", normalizedTime);
		shader->setUniform("texture", sf::Shader::CurrentTexture);
		states.shader = shader;
	}

	target.draw(label, states);
}
