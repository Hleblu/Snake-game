#include "apple.h"
#include <iostream>

Apple::Apple(Snake& Snake) : snake(&Snake), rect(sf::Vector2f(size, size)), gen(rd()) {
    genCords();
    rect.setFillColor(sf::Color(233, 67, 37));
}

void Apple::genCords()
{
    std::uniform_int_distribution<> distX(0, gridX-1);
    std::uniform_int_distribution<> distY(0, gridY-1);

    x = distX(gen);
    y = distY(gen);

    if (snake->segmentsSet.count({ x,y }) > 0) { genCords(); return; }
    else if (x == snake->segments[0].x && y == snake->segments[0].y) { genCords(); return; }
    else rect.setPosition({ x * size, y * size });
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(rect, states);
}