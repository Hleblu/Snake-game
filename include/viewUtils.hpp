#pragma once
#include "direction.hpp"
#include "randomGenerator.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

class ViewUtils
{
public:
	static void normalizeView(sf::View& view, sf::Vector2f dimensions)
	{
		const sf::Vector2f halfSize = dimensions / 2.f;
		view.setSize(dimensions);
		view.setCenter(halfSize);
	}

	static void shakeView(
		sf::View& view,
		sf::Vector2f defaultCenter,
		sf::RenderWindow& window,
		unsigned int intensivity,
		Direction::Val dir
	)
	{
		const sf::Vector2i vector = Direction::getVector(dir);
		const float offsetX = RandomGenerator::getInt(intensivity / 2u, intensivity) * vector.x;
		const float offsetY = RandomGenerator::getInt(intensivity / 2u, intensivity) * vector.y;
		view.setCenter(defaultCenter += { offsetX, offsetY });
		window.setView(view);
	}

	static void shakeView(
		sf::View& view,
		sf::Vector2f defaultCenter,
		sf::RenderWindow& window,
		unsigned int intensivity,
		Direction::Val dir,
		float timePassed,
		float duration
	)
	{
		intensivity *= std::max(0.f, 1 - timePassed / duration);
		shakeView(view, defaultCenter, window, intensivity, dir);
	}
};