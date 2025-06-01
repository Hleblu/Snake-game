#include "apple.h"

Apple::Apple(Snake& Snake) : snake(&Snake) {
	rect = sf::RectangleShape(sf::Vector2f(config->size, config->size));
	gen = std::mt19937(rd());
    updateData();
    generateNewPosition();
}

void Apple::updateData() {
    distX = std::uniform_int_distribution<>(0, config->rows - 1);
    distY = std::uniform_int_distribution<>(0, config->columns - 1);

    rect.setFillColor(sf::Color(config->currentTheme.appleColor));
    rect.setSize(sf::Vector2f(config->size, config->size));
}

void Apple::generateNewPosition()
{
    if (config->rows * config->columns == snake->getSegments().size()) return;
    do {
        x = distX(gen);
        y = distY(gen);
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

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect, states);
}