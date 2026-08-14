#include "checkeredBG.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

ChekeredBG::ChekeredBG(sf::Texture texture)
	: texture(std::move(texture)), sprite(this->texture) { }

void ChekeredBG::updateDimensions(float scale, sf::IntRect& textureRect)
{
	sprite.setScale(sf::Vector2f(scale, scale));
	sprite.setTextureRect(textureRect);
}

sf::Texture& ChekeredBG::getTexture()
{
	return texture;
}

void ChekeredBG::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}