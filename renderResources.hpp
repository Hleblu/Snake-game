#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

class RenderResources
{
	RenderResources();
public:
	static RenderResources& getInstance();
	RenderResources(const RenderResources&) = delete;
	RenderResources& operator=(const RenderResources&) = delete;

	void loadGradientShader();
	void createBackgroundTexture();

	sf::Shader gradientShader;
	sf::Texture backgroundTexture;
};

inline RenderResources& renderResources = RenderResources::getInstance();