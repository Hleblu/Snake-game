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
	void loadFloatingTextFadeShader();

	sf::Shader snakeShader;
	sf::Shader fadeShader;
	sf::Shader spriteFadeShader;
	sf::Shader floatingTextFadeShader;

	sf::Texture backgroundTexture;
	sf::Texture appleTexture;
};