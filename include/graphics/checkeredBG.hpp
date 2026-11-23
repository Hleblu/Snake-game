#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

class CheckeredBG : public sf::Drawable
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
public:
	CheckeredBG(sf::Texture texture);
	void updateDimensions(float scale, const sf::IntRect& textureRect);
	sf::Texture& getTexture();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};