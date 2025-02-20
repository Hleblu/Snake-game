#include "apple.h"
#include <iostream>

Apple::Apple(Snake& Snake) : snake(&Snake), rect(sf::Vector2f(size, size)), gen(rd()), distX(0, gridX - 1), distY(0, gridY - 1) {
    generateNewPosition();
    rect.setFillColor(sf::Color(233, 67, 37));
}

void Apple::generateNewPosition()
{
    if (gridX * gridY == snake->segments.size()) return;
    snake->segmentsSet.insert(snake->segments[0]);
    do {
        x = distX(gen);
        y = distY(gen);
    } while (snake->segmentsSet.count({ x,y }) != 0);
    snake->segmentsSet.erase(snake->segments[0]);
    rect.setPosition({ x * size, y * size });
}

bool Apple::isEaten()
{
    return (x == snake->segments[0].x && y == snake->segments[0].y) ? true : false;
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect, states);
}