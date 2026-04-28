#include "flash.hpp"
#include "configuration.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

constexpr std::uint8_t ALPHA_MAX = 115;

Flash::Flash(Configuration* config, sf::Color color)
	: config(config),
	color(color),
	alpha(0)
{
	rect.setSize({ config->width * 1.f, config->height * 1.f });
}

void Flash::updateAnim(float time)
{
	alpha = 1.f - (time - config->gameOverDelay) * ALPHA_MAX;
	auto newColor = color;
	newColor.a = alpha;
	rect.setFillColor(newColor);
}

void Flash::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect);
}
