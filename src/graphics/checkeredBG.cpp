#include "graphics/checkeredBG.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

CheckeredBG::CheckeredBG(sf::Texture texture)
	: texture(std::move(texture)), sprite(this->texture) { }

void CheckeredBG::updateDimensions(float scale, const sf::IntRect& textureRect)
{
	sprite.setScale(sf::Vector2f(scale, scale));
	sprite.setTextureRect(textureRect);
}

sf::Texture& CheckeredBG::getTexture()
{
	return texture;
}

void CheckeredBG::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}