#include "apple.h"

Apple::Apple(Snake& Snake) : snake(&Snake) {
    generateNewPosition();
}

void Apple::updateGrapicalData() {
    rect.setFillColor(sf::Color(config->currentTheme.appleColor));
    rect.setSize(sf::Vector2f(config->size, config->size));
}

void Apple::generateNewPosition()
{
    if (config->rows * config->columns == snake->getSegments().size()) return;
    do {
        x = RandomGenerator::getInt(0, config->rows - 1);
        y = RandomGenerator::getInt(0, config->columns - 1);
    } while (snake->getSegmentsHash().count({ x, y }) != 0);
    rect.setPosition({ static_cast<float>(x) * config->size, static_cast<float>(y) * config->size });
}

bool Apple::isEaten() const
{
    return rect.getGlobalBounds().contains({
        static_cast<float>(snake->getPrevSegments()[0].x) * config->size,
        static_cast<float>(snake->getPrevSegments()[0].y) * config->size
        });
}

void Apple::applyEffect() {
    snake->grow();
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect, states);
}

BonusApple::BonusApple(Snake& snake) : Apple(snake) {}

void BonusApple::updateGrapicalData() {
    rect.setFillColor(sf::Color(config->currentTheme.bonusAppleColor));
    rect.setSize(sf::Vector2f(config->size, config->size));
}

void BonusApple::applyEffect() {
    snake->grow(2);
}

HasteApple::HasteApple(Snake& snake) : Apple(snake) {}

void HasteApple::updateGrapicalData() {
    rect.setFillColor(sf::Color(config->currentTheme.bonusAppleColor));
    rect.setSize(sf::Vector2f(config->size, config->size));
}

void HasteApple::applyEffect() {
    config->delayDecreaseBonus = 0.8f;
}

std::unique_ptr<Apple> AppleFactory::createRandomApple(Snake& snake) {
    int number = RandomGenerator::getInt(1, 10);
    std::unique_ptr<Apple> apple;

    if (number <= 8) apple = std::make_unique<Apple>(snake);
    else if (number <= 9) apple = std::make_unique<HasteApple>(snake);
    else apple = std::make_unique<BonusApple>(snake);

    apple->updateGrapicalData();
    return apple;
}