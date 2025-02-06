#include "snake.h"

Snake::Snake()
{
    setDefaults();
}

void Snake::setDefaults()
{
    segments = {
        {gridX / 2, gridY / 2},
        {gridX / 2 - 1, gridY / 2},
        {gridX / 2 - 2, gridY / 2}
    };
    segmentsSet = { segments[1], segments[2] };
    direction = NONE, previousDirection = NONE, nextDirection = NONE;
    colorDecrementStep = 242 / segments.size();
    updateVertexArray();
}

bool Snake::hasCollided()
{
    if (segmentsSet.count(segments[0]) > 0) return true;

    if (segments[0].x == gridX || segments[0].x == -1
        || segments[0].y == gridY || segments[0].y == -1) return true;

    return false;
}

void Snake::grow()
{
    segments.push_back(segments.back());
    colorDecrementStep = 242 / segments.size();
}

void Snake::move()
{
    if (direction == NONE) return;

    segmentsSet.erase(segments.back());
    segmentsSet.insert(segments.front());
    for (int i = segments.size() - 1; i > 0; --i) {
        segments[i] = segments[i - 1];
    }

    switch (direction) {
    case RIGHT: segments[0].x++; break;
    case LEFT: segments[0].x--; break;
    case UP: segments[0].y--; break;
    case DOWN: segments[0].y++; break;
    default: break;
    }
    previousDirection = direction;
}

bool Snake::canUpdateDirection()
{
    return (nextDirection != direction && nextDirection % 2 != previousDirection % 2) ? true : false;
}

void Snake::updateVertexArray()
{
    sf::VertexArray vertices(sf::PrimitiveType::Triangles, vertexGrid);
    sf::Color color = snakeColor;
    for (auto& i : segments) {
        float posX = i.x * size;
        float posY = i.y * size;

        sf::Vertex* triangles = &vertices[(i.x + i.y * gridX) * 6];
        triangles[0].position = sf::Vector2f(posX, posY);
        triangles[1].position = sf::Vector2f(posX + size, posY);
        triangles[2].position = sf::Vector2f(posX + size, posY + size);
        triangles[3].position = sf::Vector2f(posX + size, posY + size);
        triangles[4].position = sf::Vector2f(posX, posY + size);
        triangles[5].position = sf::Vector2f(posX, posY);

        triangles[0].color = color;
        triangles[1].color = color;
        triangles[2].color = color;
        triangles[3].color = color;
        triangles[4].color = color;
        triangles[5].color = color;

        color.b -= colorDecrementStep;
    }
    snakeVertices = vertices;
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(snakeVertices, states);
}