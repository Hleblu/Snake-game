#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Color.hpp>

class RenderResources
{
public:
	RenderResources() = default;

	void loadSnakeShader();
	sf::Texture createCheckerboardTexture();
	void updateCheckerboardTexture(sf::Texture& texture, sf::Color first, sf::Color second);
	void loadAppleTexture();
	void loadFadeShader();
	void loadSpriteFadeShader();
	void loadFloatingTextFadeShader();

	sf::Shader snakeShader;
	sf::Shader fadeShader;
	sf::Shader spriteFadeShader;
	sf::Shader floatingTextFadeShader;

	sf::Texture appleTexture;
};