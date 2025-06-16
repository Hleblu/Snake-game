#pragma once
#include <SFML/Graphics/Color.hpp>
#include <array>

class Configuration
{
	Configuration();

	struct Theme {
		sf::Color mainColor;
		sf::Color secondColor;
		sf::Color appleColor;
		sf::Color bonusAppleColor;
		sf::Color snakeColor;
		sf::Color snakeColorEnd;
		sf::Color obstacleColor;
	};

	const Theme classicTheme = {
		{114, 183, 106},
		{172, 206, 94},
		{233, 67, 37},
		{247, 158, 2},
		{41, 74, 242},
		{10, 45, 216},
		{60, 60, 60}
	};

	const Theme classicSoftTheme = {
		{210, 228, 200},
		{190, 210, 180},
		{220, 60, 60},
		{255, 180, 70},
		{60, 120, 180},      
		{30, 90, 150},
		{80, 80, 80}
	};

	const Theme grayscaleTheme = {
		{230, 230, 230},
		{200, 200, 200},
		{180, 50, 50},
		{220, 190, 100},
		{80, 80, 80},
		{40, 40, 40},
		{30, 60, 100}
	};

	const Theme pastelBlueTheme = {
		{215, 225, 240},
		{185, 205, 225},
		{240, 100, 100},
		{255, 190, 110},
		{90, 150, 200},
		{60, 120, 170},
		{70, 70, 90}
	};

	const Theme earthTheme = {
		{222, 215, 195},
		{200, 190, 170},
		{180, 60, 45},
		{240, 180, 90},
		{110, 90, 60},
		{70, 50, 30},
		{40, 30, 20}
	};

	const Theme forestTheme = {
		{200, 220, 200},
		{180, 200, 180},
		{210, 80, 50},
		{255, 180, 60},
		{60, 120, 60},
		{40, 90, 40},
		{20, 50, 20}
	};

	const Theme pastelPinkTheme = {
		{245, 230, 235},
		{230, 210, 215},
		{255, 105, 130},
		{255, 200, 100},
		{200, 120, 160},
		{150, 90, 130},
		{90, 60, 80}
	};

public:
	static Configuration& getInstance();
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
	template<class T, std::size_t N> void cycleOptions(T& current, const std::array<T, N>& options, int& index) {
		if (N == 0) return;
		index = (index + 1) % N;
		current = options[index];
	}

	std::uint16_t size = 40;
	std::uint16_t rows = 18;
	std::uint16_t columns = 15;
	const std::uint16_t width = 720;
	const std::uint16_t height = 600;

	const std::array<std::uint16_t, 3> cellSizes = { 40, 60, 30 };

	const float animationFrameTime = 45.0f;
	const float delayDecreaseStep = 0.995f;
	float delay = 0.115f;
	float delayDecreaseBonus = 1.f;

	std::array<float, 3> speedOptions = { 0.115f, 0.135f, 0.095f };

	const Theme* currentTheme = &classicTheme;
	const std::array<const Theme*, 7> themes = { 
		&classicTheme,
		&classicSoftTheme,
		&grayscaleTheme,
		&pastelBlueTheme,
		&earthTheme,
		&forestTheme,
		&pastelPinkTheme 
	};

	const sf::Color textColor1 = { 255, 255, 255 };
	const sf::Color textColor2 = { 30, 30, 30 };

	const std::array<bool, 2> boolOptions = { true, false };
	bool obstaclesEnabled = true;

	const std::array<const char*, 3> speedOptionsLabels = { "Speed: Default", "Speed: Slow", "Speed: Fast" };
	const std::array<const char*, 3> fieldSizeOptionsLabels = { "Field size: Default", "Field size: Small", "Field size: Big" };
	const std::array<const char*, 2> obstaclesOptionsLabels = { "Obstacles: Enabled", "Obstacles: Disabled" };
};

inline Configuration& config = Configuration::getInstance();