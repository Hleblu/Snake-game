#pragma once
#include <array>
#include <SFML/Graphics/Color.hpp>
#include <string>

class Configuration
{
public:
	struct Theme {
		sf::Color mainColor;
		sf::Color secondColor;
		sf::Color appleColor;
		sf::Color snakeColor;
		sf::Color snakeColorEnd;
		sf::Color obstacleColor;
	};

	struct GridOption {
		std::uint16_t size;
		std::uint16_t columns;
		std::uint16_t rows;
		std::string label;
	};

	struct SpeedOption {
		float delay;
		std::string label;
	};

	const std::uint16_t width = 720;
	const std::uint16_t height = 600;
	const sf::Color textBaseColor = { 255, 255, 255 };
	const sf::Color textOutlineColor = { 30, 30, 30 };
	const float delayDecreaseStep = 0.995f;
	const float gameOverDelay = 0.5f;
private:
	template<std::size_t N> void cycleIndex(size_t& index) {
		index = (index + 1) % N;
	}

	std::size_t currentThemeIndex = 0;
	std::size_t currentGridSettingsIndex = 0;
	std::size_t currentSpeedIndex = 0;

	bool obstaclesEnabled = true;

	inline static const std::array<Theme, 7> themeOptions = { {
			//classic
			{ {114, 183, 106}, {172, 206, 94}, {233, 67, 37}, {41, 74, 242}, {10, 45, 216},	{60, 60, 60} },
			//classic soft
			{ {210, 228, 200}, {190, 210, 180},	{220, 60, 60}, {60, 120, 180}, {30, 90, 150}, {80, 80, 80} },
			//grayscale
			{ {230, 230, 230}, {200, 200, 200}, {180, 50, 50}, {80, 80, 80}, {40, 40, 40}, {30, 60, 100} },
			//pastel blue
			{ {215, 225, 240}, {185, 205, 225},	{240, 100, 100}, {90, 150, 200}, {60, 120, 170}, {70, 70, 90} },
			//earth
			{ {222, 215, 195}, {200, 190, 170}, {180, 60, 45}, {110, 90, 60}, {70, 50, 30}, {40, 30, 20} },
			//forest
			{ {200, 220, 200}, {180, 200, 180},	{210, 80, 50}, {60, 120, 60}, {40, 90, 40}, {20, 50, 20} },
			//pastel pink
			{ {245, 230, 235}, {230, 210, 215}, {255, 105, 130}, {200, 120, 160}, {150, 90, 130}, {90, 60, 80} }
	} };

	inline static const std::array<GridOption, 3> gridOptions = { {
		{ 40, 18, 15, "Field size: Default" },
		{ 60, 12, 10, "Field size: Small" },
		{ 30, 24, 20, "Field size: Big" }
	} };

	inline static const std::array<SpeedOption, 3> speedOptions = { {
		{ 0.115f, "Speed: Default" },
		{ 0.135f, "Speed: Slow" },
		{ 0.095f, "Speed: Fast" }
	} };

	inline static const std::array<std::string, 2> obstaclesOptionsLabels = { "Obstacles: Enabled", "Obstacles: Disabled" };

public:
	const Theme& getCurrentTheme() const { return themeOptions[currentThemeIndex]; }

	const GridOption& getGridSettings() const { return gridOptions[currentGridSettingsIndex]; }
	std::uint16_t getCellSize() const { return getGridSettings().size; }
	std::uint16_t getRows() const { return getGridSettings().rows; }
	std::uint16_t getColumns() const { return getGridSettings().columns; }
	std::uint16_t getTotalSize() const { return getRows() * getColumns(); }

	float getStartDelay() const { return speedOptions[currentSpeedIndex].delay; }

	bool areObstaclesEnabled() const { return obstaclesEnabled; }

	const std::string& getCurrentGridLabel() const { return gridOptions[currentGridSettingsIndex].label; }
	const std::string& getCurrentSpeedLabel() const { return speedOptions[currentSpeedIndex].label; }
	const std::string& getCurrentObstaclesLabel() const { return obstaclesOptionsLabels[obstaclesEnabled ? 0 : 1]; }

	void cycleTheme() { cycleIndex<themeOptions.size()>(currentThemeIndex); }
	void cycleGridSize() { cycleIndex<gridOptions.size()>(currentGridSettingsIndex); }
	void cycleSpeed() { cycleIndex<speedOptions.size()>(currentSpeedIndex); }
	void toggleObstacles() { obstaclesEnabled = !obstaclesEnabled; }

	float getParticleSize() { return getCellSize() / 5.f; }
	float getParticleSpeedMin() { return getCellSize() * 1.1f; }
	float getParticleSpeedMax() { return getCellSize() * 3.f; }

	std::uint16_t getSnakeDefSize() { return std::max(getColumns() / 5, 3); }
};