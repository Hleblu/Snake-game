#include "snake.h"

Snake::Snake()
{
    segmentsVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    restoreDefaultValues();
}

void Snake::restoreDefaultValues()
{
    short int centerX = config->rows / 2;
    short int centerY = config->columns / 2;

    segments = {
        {centerX, centerY},
        {static_cast<short>(centerX - 1), centerY},
        {static_cast<short>(centerX - 2), centerY}
    };
    previousSegments = segments;
    segmentsSet = { segments[0], segments[1], segments[2] };

    direction = NONE, previousDirection = NONE, nextDirection = NONE;
    firstMove = true;
    hashDelay = 0;

    segmentsVertices.clear();
    segmentsVertices.resize(config->rows * config->columns * 6);

    updateVertices();
	updateTexCoords();
}

bool Snake::hasCollided() const
{
    return segmentsSet.count(segments[0]) > 1 
        || static_cast<unsigned>(segments[0].x) > config->rows - 1 
        || static_cast<unsigned>(segments[0].y) > config->columns - 1;
}

void Snake::grow(int size)
{
    hashDelay += size;
    for (int i = 0; i < size; ++i) {
        segments.emplace_back(segments.back());
        previousSegments.emplace_back(previousSegments.back());
    }
	updateTexCoords();
}

void Snake::move()
{
    if (direction == NONE) return;
    
    if (!firstMove) { 
        previousSegments.pop_back();
        previousSegments.emplace_front(segments.front());
    }
    else firstMove = false;

    if (hashDelay == 0)
        segmentsSet.erase(segments.back());
    else hashDelay -= 1;
    
    segments.pop_back();
    segments.emplace_front(segments.front());

    switch (direction) {
    case RIGHT: segments[0].x++; break;
    case LEFT: segments[0].x--; break;
    case UP: segments[0].y--; break;
    case DOWN: segments[0].y++; break;
    default: break;
    }

    segmentsSet.insert(segments.front());
    previousDirection = direction;
}

bool Snake::canUpdateDirection() const
{
    return nextDirection != direction && nextDirection % 2 != previousDirection % 2;
}

const std::deque<Snake::Segment>& Snake::getSegments() const 
{
    return Snake::segments;
}

const std::deque<Snake::Segment>& Snake::getPrevSegments() const
{
    return Snake::previousSegments;
}

const std::unordered_multiset<Snake::Segment, Snake::SegmentHash>& Snake::getSegmentsHash() const
{
    return Snake::segmentsSet;
}

void Snake::updateTexCoords()
{
	for (size_t i = 0; i < segments.size(); ++i) {
        const float normalizedPosition = static_cast<float>(i) / (segments.size() - 1);
        sf::Vertex* triangles = &segmentsVertices[i * 6];
        triangles[0].texCoords = { 0, normalizedPosition };
        triangles[1].texCoords = { 1, normalizedPosition };
        triangles[2].texCoords = triangles[0].texCoords;
        triangles[3].texCoords = triangles[1].texCoords;
        triangles[4].texCoords = triangles[0].texCoords;
        triangles[5].texCoords = triangles[1].texCoords;
	}
}

void Snake::updateVertices(float dt)
{
    for (size_t i = 0; i < segments.size(); ++i) {
        const float posX = (previousSegments[i].x + (segments[i].x - previousSegments[i].x) * dt) * config->size;
        const float posY = (previousSegments[i].y + (segments[i].y - previousSegments[i].y) * dt) * config->size;
		const float posXEnd = posX + config->size;
		const float posYEnd = posY + config->size;

        sf::Vertex* triangles = &segmentsVertices[i * 6];
        triangles[0].position = { posX, posY };
        triangles[1].position = { posXEnd, posY };
        triangles[2].position = { posXEnd, posYEnd };
        triangles[3].position = triangles[2].position;
        triangles[4].position = { posX, posYEnd };
        triangles[5].position = triangles[0].position;
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(segmentsVertices, states);
}