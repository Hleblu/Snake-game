#pragma once
#include <SFML/Graphics.hpp>

class Renderer
{
	Renderer();
public:
	static Renderer& getInstance();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	void loadGradientShader();
	void createBackgroundTexture();

	sf::Shader gradientShader;
	sf::Texture backgroundTexture;
};

inline Renderer& renderer = Renderer::getInstance();