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

	unsigned int size = 40;
	unsigned int rows = 17;
	unsigned int columns = 15;
	unsigned int width = 680;
	unsigned int height = 600;

	float delay = 0.115f;
	sf::Color mainColor = {114, 183, 106};
	sf::Color secondColor = { 172, 206, 94 };
	sf::Color appleColor = { 233, 67, 37 };
	sf::Color snakeColor = { 41, 74, 242 };
	sf::Color snakeColorEnd = { 10, 45, 216 };
};