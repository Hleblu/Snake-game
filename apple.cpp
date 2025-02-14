#include "apple.h"
#include <iostream>

Apple::Apple(Snake& Snake) : snake(&Snake), rect(sf::Vector2f(size, size)), gen(rd()), distX(0, gridX - 1), distY(0, gridY - 1) {
    generateNewPosition();
    rect.setFillColor(sf::Color(233, 67, 37));
}

void Apple::generateNewPosition()
{
    x = distX(gen);
    y = distY(gen);

    if (snake->segmentsSet.count({ x,y }) > 0 
        || x == snake->segments[0].x && y == snake->segments[0].y) { generateNewPosition(); return; }
    else rect.setPosition({ x * size, y * size });
}

bool Apple::isEaten()
{
    return (x == snake->segments[0].x && y == snake->segments[0].y) ? true : false;
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect, states);
}