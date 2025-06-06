#pragma once
#include <SFML/Graphics.hpp>
class Configuration
{
	Configuration();
	static Configuration* instance;

	struct Theme {
		sf::Color mainColor;
		sf::Color secondColor;
		sf::Color appleColor;
		sf::Color bonusAppleColor;
		sf::Color snakeColor;
		sf::Color snakeColorEnd;
	};

	const Theme classicTheme = {
		{114, 183, 106},
		{172, 206, 94},
		{233, 67, 37},
		{247, 158, 2},
		{41, 74, 242},
		{10, 45, 216},
	};

	const Theme classicSoftTheme = {
		{210, 228, 200},
		{190, 210, 180},
		{220, 60, 60},
		{255, 180, 70},
		{60, 120, 180},      
		{30, 90, 150}
	};

	const Theme grayscaleTheme = {
		{230, 230, 230},
		{200, 200, 200},
		{180, 50, 50},
		{220, 190, 100},
		{80, 80, 80},
		{40, 40, 40}
	};

	const Theme pastelBlueTheme = {
		{215, 225, 240},
		{185, 205, 225},
		{240, 100, 100},
		{255, 190, 110},
		{90, 150, 200},
		{60, 120, 170}
	};

	const Theme earthTheme = {
		{222, 215, 195},
		{200, 190, 170},
		{180, 60, 45},
		{240, 180, 90},
		{110, 90, 60},
		{70, 50, 30}
	};

	const Theme forestTheme = {
		{200, 220, 200},
		{180, 200, 180},
		{210, 80, 50},
		{255, 180, 60},
		{60, 120, 60},
		{40, 90, 40}
	};

	const Theme pastelPinkTheme = {
		{245, 230, 235},
		{230, 210, 215},
		{255, 105, 130},
		{255, 200, 100},
		{200, 120, 160},
		{150, 90, 130}
	};

public:
	static Configuration* getInstance();
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
	template<class T> void cycleOptions(T& current, const std::vector<T>& options, int& index) {
		if (options.empty()) return;
		index = (index + 1) % options.size();
		current = options[index];
	}

	short unsigned int size = 40;
	short unsigned int rows = 18;
	short unsigned int columns = 15;
	short unsigned int width = 720;
	short unsigned int height = 600;

	std::vector<short unsigned int> cellSizes = { 40, 60, 30 };

	float animationFrameTime = 45.0f;
	float delayDecreaseStep = 0.995f;
	float delay = 0.115f;
	float delayDecreaseBonus = 1.f;

	std::vector<float> speedOptions = { 0.115f, 0.135f, 0.095f };

	const Theme* currentTheme = &classicTheme;
	std::vector<const Theme*> themes = { 
		&classicTheme,
		&classicSoftTheme,
		&grayscaleTheme,
		&pastelBlueTheme,
		&earthTheme,
		&forestTheme,
		&pastelPinkTheme 
	};

	sf::Color textColor1 = {255, 255, 255};
	sf::Color textColor2 = { 30, 30, 30 };

	std::vector<std::string> speedOptionsLabels = { "Speed: Default", "Speed: Slow", "Speed: Fast" };
	std::vector<std::string> fieldSizeOptionsLabels = { "Field size: Default", "Field size: Small", "Field size: Big"};
};