#include "snake.h"

Snake::Snake()
{
    segmentsVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    segmentsSet.reserve(gridX * gridY - 1);
    restoreDefaultValues();
}

void Snake::restoreDefaultValues()
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
    return segmentsSet.count(segments[0]) > 0 
        || unsigned(segments[0].x) > gridX - 1 
        || unsigned(segments[0].y) > gridY - 1;
}

void Snake::grow()
{
    segments.emplace_back(segments.back());
    colorDecrementStep = 242 / segments.size();
}

void Snake::move()
{
    if (direction == NONE) return;

    segmentsSet.erase(segments.back());
    segmentsSet.insert(segments.front());
    segments.pop_back();
    segments.emplace_front(segments.front());

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
    return nextDirection != direction && nextDirection % 2 != previousDirection % 2;
}

void Snake::updateVertexArray()
{
    if (segmentsVertices.getVertexCount() != segments.size() * 6) {
		segmentsVertices.resize(segments.size() * 6);

        sf::Color color = segmentColor;
        for (int i = 0; i < segments.size(); ++i) {
            sf::Vertex* triangles = &segmentsVertices[i * 6];
			for (int j = 0; j < 6; ++j) {
				triangles[j].color = color;
			}
            color.b -= colorDecrementStep;
        }
    }
    for (int i = 0; i < segments.size(); ++i) {
        float posX = segments[i].x * size;
        float posY = segments[i].y * size;

        sf::Vertex* triangles = &segmentsVertices[i * 6];
        triangles[0].position = sf::Vector2f(posX, posY);
        triangles[1].position = sf::Vector2f(posX + size, posY);
        triangles[2].position = sf::Vector2f(posX + size, posY + size);
        triangles[3].position = sf::Vector2f(posX + size, posY + size);
        triangles[4].position = sf::Vector2f(posX, posY + size);
        triangles[5].position = sf::Vector2f(posX, posY);
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(segmentsVertices, states);
}