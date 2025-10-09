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

public:
	Apple();
	void generateNewPosition();
	bool isEaten() const;
	virtual void updateGraphicalData();
	virtual void applyEffect(Snake& snake);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual ~Apple() {};
};

class BonusApple : public Apple
{
public:
	void updateGraphicalData() override;
	void applyEffect(Snake& snake) override;
};

class HasteApple : public BonusApple
{
public:
	void applyEffect(Snake& snake) override;
};

class SlownessApple : public BonusApple
{
public:
	void applyEffect(Snake& snake) override;
};

class AppleFactory
{
public:
	static std::unique_ptr<Apple> createRandomApple();
};