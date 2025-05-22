#pragma once
#include <SFML/Graphics.hpp>
class Configuration
{
	Configuration();
	static Configuration* instance;
public:
	static Configuration* getInstance();
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;

	short unsigned int size = 40;
	short unsigned int rows = 17;
	short unsigned int columns = 15;
	short unsigned int width = 680;
	short unsigned int height = 600;

	float animationFrameTime = 45.0f;
	float speedDecreaseStep = 0.995f;
	float delay = 0.115f;

	sf::Color mainColor = { 114, 183, 106 };
	sf::Color secondColor = { 172, 206, 94 };
	sf::Color appleColor = { 233, 67, 37 };
	sf::Color snakeColor = { 41, 74, 242 };
	sf::Color snakeColorEnd = { 10, 45, 216 };
	sf::Color textColor1 = { 255, 255, 255 };
	sf::Color textColor2 = { 0, 0, 0 };
};