#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include "snake.h"
#include "collisionManager.h"
#include "cell.h"
#include "randomGenerator.h"

class Apple : public sf::Drawable
{
protected:
	sf::RectangleShape rect;
	Cell coords;
	Configuration* config = Configuration::getInstance();
	CollisionManager* collisionManager = CollisionManager::getInstance();

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