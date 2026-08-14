#pragma once
#include "direction.hpp"
#include "randomGenerator.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

class ViewUtils
{
public:
	static void normalizeView(sf::View& view, sf::Vector2f target, sf::Vector2f current)
	{
		const float targetRatio = target.x / target.y;
		const float currentRatio = current.x / current.y;

		sf::FloatRect viewRect({ 0.f, 0.f }, { 1.f, 1.f });

		if (targetRatio < currentRatio)
		{
			viewRect.size.x = targetRatio / currentRatio;
			viewRect.position.x = (1.f - viewRect.size.x) / 2.f;
		}
		else {
			viewRect.size.y = currentRatio / targetRatio;
			viewRect.position.y = (1.f - viewRect.size.y) / 2.f;
		}

		view.setSize(target);
		view.setCenter(target / 2.f);
		view.setViewport(viewRect);
	}
		
	static void shakeView(
		sf::View& view,
		const sf::Vector2f defaultCenter,
		float intensity,
		Direction::Val dir
	)
	{
		const sf::Vector2f vector{ Direction::getVector(dir) };

		const float offsetX = RandomGenerator::getFloat(intensity / 2.f, intensity) * vector.x;
		const float offsetY = RandomGenerator::getFloat(intensity / 2.f, intensity) * vector.y;
		
		view.setCenter(defaultCenter + sf::Vector2f{ offsetX, offsetY });
	}

	static void shakeView(
		sf::View& view,
		const sf::Vector2f defaultCenter,
		float intensity,
		Direction::Val dir,
		float timePassed,
		float duration
	)
	{
		intensity *= std::max(0.f, 1.f - timePassed / duration);
		shakeView(view, defaultCenter, intensity, dir);
	}

	static float getSameRatioDiffMult(sf::Vector2f target, sf::Vector2f current)
	{
		const float targetRatio = target.x / target.y;
		const float currentRatio = current.x / current.y;
		const float diff = targetRatio / currentRatio;

		if (targetRatio < currentRatio)
		{
			return current.y / target.y;
		}
		else {
			return current.x / target.x;
		}
	}
};