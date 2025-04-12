#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
class Renderer
{
	Renderer();
	static Renderer* instance;
	Configuration* config = Configuration::getInstance();
public:
	static Renderer* getInstance();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	void loadGradientShader();
	void createBackgroundTexture();

	sf::Shader gradientShader;
	sf::Texture backgroundTexture;
};

