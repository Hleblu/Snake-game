#pragma once
#include "cell.hpp"
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Snake;

class IAppleEffect;

class Apple : public sf::Drawable
{
	sf::Sprite sprite;
	Cell coords;
	std::unique_ptr<IAppleEffect> effect;

public:
	Apple(std::unique_ptr<IAppleEffect> eff, sf::Texture& texture);
	void generateNewPosition();
	bool isEaten() const;
	float getSpeedBonus() const;
	void applyEffect(Snake& snake) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class AppleFactory
{
public:
	static std::unique_ptr<Apple> createRandomApple(sf::Texture& texture);
};

class IAppleEffect
{
public:
	virtual ~IAppleEffect() = default;
	virtual void apply(Snake& snake) const = 0;
	virtual float getSpeedBonus() const = 0;
	virtual sf::IntRect getTextureRect() const = 0;
};

class BasicEffect : public IAppleEffect
{
	void apply(Snake& snake) const override;
	float getSpeedBonus() const override;
	sf::IntRect getTextureRect() const override;
};

class BonusEffect : public IAppleEffect
{
	void apply(Snake& snake) const override;
	float getSpeedBonus() const override;
	sf::IntRect getTextureRect() const override;
};

class HasteEffect : public IAppleEffect
{
	void apply(Snake& snake) const override;
	float getSpeedBonus() const override;
	sf::IntRect getTextureRect() const override;
};

class SlownessEffect : public IAppleEffect
{
	void apply(Snake& snake) const override;
	float getSpeedBonus() const override;
	sf::IntRect getTextureRect() const override;
};