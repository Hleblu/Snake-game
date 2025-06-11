#include "apple.h"

Apple::Apple() {
    generateNewPosition();
}

void Apple::updateGraphicalData() {
    rect.setFillColor(sf::Color(config->currentTheme->appleColor));
    rect.setSize(sf::Vector2f(config->size, config->size));
}

void Apple::generateNewPosition()
{
    collisionManager->setFree(coords, APPLE);
    if (collisionManager->numberOfOccupied() >= config->rows * config->columns) return;

    const short int xMax = config->rows - 1;
    const short int yMax = config->columns - 1;
    do {
        coords.x = RandomGenerator::getInt(0, xMax);
        coords.y = RandomGenerator::getInt(0, yMax);
    } while (collisionManager->isCellOccupied(coords));

    rect.setPosition(sf::Vector2f(coords.x * config->size, coords.y * config->size));
    collisionManager->setOccupied(coords, APPLE);
}

bool Apple::isEaten() const
{
    return collisionManager->checkCellType(coords, SNAKE_HEAD);
}

void Apple::applyEffect(Snake& snake) {
    snake.grow();
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect, states);
}

void BonusApple::updateGraphicalData() {
    rect.setFillColor(sf::Color(config->currentTheme->bonusAppleColor));
    rect.setSize(sf::Vector2f(config->size, config->size));
}

void BonusApple::applyEffect(Snake& snake) {
    snake.grow(2);
}

void HasteApple::applyEffect(Snake& snake) {
    snake.grow();
    config->delayDecreaseBonus = 0.8f;
}

void SlownessApple::applyEffect(Snake& snake) {
    snake.grow();
    config->delayDecreaseBonus = 1.2f;
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