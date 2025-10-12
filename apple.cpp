#include "apple.hpp"
#include "collisionManager.hpp"
#include "configuration.hpp"
#include "randomGenerator.hpp"
#include "snake.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Apple::Apple() : speedBonus(1.f), color(config.currentTheme->appleColor) {
    generateNewPosition();
}

void Apple::updateGraphicalData() {
    rect.setFillColor(sf::Color(color));
    rect.setSize(sf::Vector2f(config.size, config.size));
}

const float Apple::getSpeedBonus() const
{
    return speedBonus;
}

void Apple::generateNewPosition()
{
    collisionManager.setFree(coords, ObjectType::APPLE);
    if (collisionManager.numberOfOccupied() >= config.rows * config.columns) return;

    const std::int16_t xMax = config.rows - 1;
    const std::int16_t yMax = config.columns - 1;
    do {
        coords.x = RandomGenerator::getInt(0, xMax);
        coords.y = RandomGenerator::getInt(0, yMax);
    } while (collisionManager.isCellOccupied(coords));

    rect.setPosition(sf::Vector2f(coords.x * config.size, coords.y * config.size));
    collisionManager.setOccupied(coords, ObjectType::APPLE);
}

bool Apple::isEaten() const
{
    return collisionManager.checkCellType(coords, ObjectType::SNAKE_HEAD);
}

void Apple::applyEffect(Snake& snake) {
    snake.grow();
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect, states);
}

BonusApple::BonusApple() {
    speedBonus = 1.f;
    color = config.currentTheme->bonusAppleColor;
}

void BonusApple::applyEffect(Snake& snake) {
    snake.grow(2);
}

HasteApple::HasteApple()
{
    speedBonus = 0.8f;
    color = config.currentTheme->bonusAppleColor;
}

void HasteApple::applyEffect(Snake& snake) {
    snake.grow();
}

SlownessApple::SlownessApple()
{
    speedBonus = 1.2f;
    color = config.currentTheme->bonusAppleColor;
}

void SlownessApple::applyEffect(Snake& snake) {
    snake.grow();
}

std::unique_ptr<Apple> AppleFactory::createRandomApple() {
    const int number = RandomGenerator::getInt(1, 30);
    std::unique_ptr<Apple> apple;

    if (number <= 27) apple = std::make_unique<Apple>();
    else if (number <= 28) apple = std::make_unique<HasteApple>();
    else if (number <= 29) apple = std::make_unique<BonusApple>();
    else apple = std::make_unique<SlownessApple>();

    apple->updateGraphicalData();
    return apple;
}