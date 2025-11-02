#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

class RenderResources
{
public:
	RenderResources() = default;

	void loadSnakeShader();
	void createBackgroundTexture();
	void loadAppleTexture();
	void loadFadeShader();
	void loadSpriteFadeShader();

	sf::Shader snakeShader;
	sf::Shader fadeShader;
	sf::Shader spriteFadeShader;
	sf::Texture backgroundTexture;
	sf::Texture appleTexture;
};