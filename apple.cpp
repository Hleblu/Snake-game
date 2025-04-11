#include "apple.h"

Apple::Apple(Snake& Snake) : snake(&Snake), config(snake->config) {
	rect = sf::RectangleShape(sf::Vector2f(config->size, config->size));
	gen = std::mt19937(rd());
	distX = std::uniform_int_distribution<>(0, config->rows - 1);
	distY = std::uniform_int_distribution<>(0, config->columns - 1);
    generateNewPosition();
    rect.setFillColor(sf::Color(config->appleColor));
}

void Apple::generateNewPosition()
{
    if (config->rows * config->columns == snake->segments.size()) return;
    do {
        x = distX(gen);
        y = distY(gen);
    } while (snake->segmentsSet.count({ x,y }) != 0);
    rect.setPosition({ x * config->size, y * config->size });
}

bool Apple::isEaten()
{
    return rect.getGlobalBounds().contains({snake->previousSegments[0].x * config->size, snake->previousSegments[0].y * config->size});
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect, states);
}