#pragma once
#include "cell.hpp"
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Snake;

class Apple : public sf::Drawable
{
protected:
	sf::RectangleShape rect;
	Cell coords;
	float speedBonus;
	sf::Color color;

public:
	Apple();
	void generateNewPosition();
	bool isEaten() const;
	void updateGraphicalData();
	virtual const float getSpeedBonus() const;
	virtual void applyEffect(Snake& snake);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual ~Apple() {};
};

class BonusApple : public Apple
{
public:
	BonusApple();
	void applyEffect(Snake& snake) override;
};

class HasteApple : public Apple
{
public:
	HasteApple();
	void applyEffect(Snake& snake) override;
};

class SlownessApple : public Apple
{
public:
	SlownessApple();
	void applyEffect(Snake& snake) override;
};

class AppleFactory
{
public:
	static std::unique_ptr<Apple> createRandomApple();
};