#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include "snake.h"
#include <random>

class Apple : public sf::Drawable
{
protected:
	Snake* snake;
	sf::RectangleShape rect;
	short int x, y;
	Configuration* config = Configuration::getInstance();

public:
	Apple(Snake& snake);
	void generateNewPosition();
	bool isEaten() const;
	virtual void updateGraphicalData();
	virtual void applyEffect();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual ~Apple() {};
};

class BonusApple : public Apple
{
public:
	BonusApple(Snake& snake);
	void updateGraphicalData() override;
	void applyEffect() override;
};

class HasteApple : public Apple
{
public:
	HasteApple(Snake& snake);
	void updateGraphicalData() override;
	void applyEffect() override;
};

class AppleFactory
{
public:
	static std::unique_ptr<Apple> createRandomApple(Snake& snake);
};

class RandomGenerator 
{
public:
	static std::mt19937& getGenerator() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return gen;
	}

	static int getInt(int min, int max) {
		std::uniform_int_distribution<> dist(min, max);
		return dist(getGenerator());
	}
};