#include "collisionManager.hpp"
#include "configuration.hpp"
#include "randomGenerator.hpp"
#include "snake.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

constexpr std::size_t VERTEX_PER_TRIANGLE = 6;

Snake::Snake()
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    jointVertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    restoreDefaultValues();
}

sf::Vector2f Snake::getPositionInfront() const
{
    const Cell& head = segments.front();

    const float x = head.x * config.getCellSize() + config.getCellSize() / 2.f;
    const float y = head.y * config.getCellSize() + config.getCellSize() / 2.f;

    return sf::Vector2f(x, y);
}

void Snake::restoreDefaultValues()
{
    const std::int16_t centerX = config.getRows() / 2;
    const std::int16_t centerY = config.getColumns() / 2;

    segments = {
        {centerX, centerY},
        {centerX - 1, centerY},
        {centerX - 2, centerY}
    };
    prevSegments = segments;

    collisionManager.setOccupied(segments[0], ObjectType::SNAKE_HEAD);
    collisionManager.setOccupied(segments[1], ObjectType::SNAKE_TAIL);
    collisionManager.setOccupied(segments[2], ObjectType::SNAKE_TAIL);

    direction = Direction::NONE, prevDirection = Direction::NONE, nextDirection = Direction::NONE;
    firstMove = true;
    tailCollisionDelay = 0;

    vertices.clear();
    vertices.resize(config.getRows() * config.getColumns() * VERTEX_PER_TRIANGLE);
    jointVertices.clear();

    updateVertices();
	updateTexCoords();
}

bool Snake::hasCollided() const
{
    const Cell& head = segments.front();
    return collisionManager.checkCellType(head, ObjectType::SNAKE_TAIL)
        || collisionManager.checkCellType(head, ObjectType::OBSTACLE)
        || collisionManager.isOutOfBorders(head);
}

bool Snake::firstTimeMoving() const
{
    return firstMove;
}

void Snake::grow(const int size)
{
    tailCollisionDelay += size;
    const size_t newSize = segments.size() + size;
    segments.resize(newSize, prevSegments.back());
    prevSegments.resize(newSize, prevSegments.back());
	updateTexCoords();
}

void Snake::move()
{
    if (direction == Direction::NONE) return;
    
    if (!firstMove) { 
        prevSegments.pop_back();
        prevSegments.emplace_front(segments.front());
    }
    else firstMove = false;

    if (tailCollisionDelay == 0)
        collisionManager.setFree(segments.back(), ObjectType::SNAKE_TAIL);
    else tailCollisionDelay -= 1;
    
    segments.pop_back();
    segments.emplace_front(segments.front());
    collisionManager.changeTypes(segments.front(), ObjectType::SNAKE_HEAD, ObjectType::SNAKE_TAIL);

    switch (direction) {
        case Direction::RIGHT:
            segments[0].x++;
            break;
        case Direction::LEFT:
            segments[0].x--;
            break;
        case Direction::UP:
            segments[0].y--;
            break;
        case Direction::DOWN:
            segments[0].y++;
            break;
        default:
            break;
    }

    collisionManager.setOccupied(segments.front(), ObjectType::SNAKE_HEAD);
    prevDirection = direction;

    updateJointVertices();
}

void Snake::setNextDirection(Direction dir)
{
    nextDirection = dir;
}

bool Snake::updateDirection()
{
    bool canUpdate = canUpdateDirection();
    if (canUpdate)
        direction = nextDirection;
    return canUpdate;
}

bool Snake::canUpdateDirection() const
{
    return nextDirection != direction 
        && static_cast<int>(nextDirection) % 2 != static_cast<int>(prevDirection) % 2;
}

const size_t Snake::getSize() const
{
    return segments.size();
}

void Snake::updateTexCoords()
{
	for (size_t i = 0; i < segments.size(); ++i) {
        const float normalizedPosition = static_cast<float>(i) / (segments.size() - 1);
        sf::Vertex* triangles = &vertices[i * VERTEX_PER_TRIANGLE];
        const sf::Vector2f texCoord = { 0, normalizedPosition };
        triangles[0].texCoords = texCoord;
        triangles[1].texCoords = texCoord;
        triangles[2].texCoords = texCoord;
        triangles[3].texCoords = texCoord;
        triangles[4].texCoords = texCoord;
        triangles[5].texCoords = texCoord;
	}
}

void Snake::updateVertices(const float dt)
{
    for (size_t i = 0; i < segments.size(); ++i) {
        const float posX = (prevSegments[i].x + (segments[i].x - prevSegments[i].x) * dt) * config.getCellSize();
        const float posY = (prevSegments[i].y + (segments[i].y - prevSegments[i].y) * dt) * config.getCellSize();
		const float posXEnd = posX + config.getCellSize();
		const float posYEnd = posY + config.getCellSize();

        sf::Vertex* triangles = &vertices[i * VERTEX_PER_TRIANGLE];
        triangles[0].position = { posX, posY };
        triangles[1].position = { posXEnd, posY };
        triangles[2].position = { posXEnd, posYEnd };
        triangles[3].position = { posXEnd, posYEnd };
        triangles[4].position = { posX, posYEnd };
        triangles[5].position = { posX, posY };
    }
}

void Snake::updateJointVertices()
{
    jointVertices.clear();

    for (size_t i = 1; i < getSize(); ++i) {
        const Cell prevSeg = segments[i - 1];
        const Cell thisSeg = segments[i];
        const Cell nextSeg = prevSegments[i];

        if (prevSeg.x != nextSeg.x && prevSeg.y != nextSeg.y) {
            const float posX = thisSeg.x * config.getCellSize();
            const float posY = thisSeg.y * config.getCellSize();
            const float posXEnd = posX + config.getCellSize();
            const float posYEnd = posY + config.getCellSize();
            const float normalizedPosition = static_cast<float>(i) / (segments.size() - 1);
            const sf::Vector2f texCoord = { 0, normalizedPosition };

            jointVertices.append({ { posX, posY }, sf::Color::White, texCoord });
            jointVertices.append({ { posXEnd, posY }, sf::Color::White, texCoord });
            jointVertices.append({ { posXEnd, posYEnd }, sf::Color::White, texCoord });
            jointVertices.append({ { posXEnd, posYEnd }, sf::Color::White, texCoord });
            jointVertices.append({ { posX, posYEnd }, sf::Color::White, texCoord });
            jointVertices.append({ { posX, posY }, sf::Color::White, texCoord });
        }
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(jointVertices, states);
    target.draw(&vertices[0], getSize() * VERTEX_PER_TRIANGLE, vertices.getPrimitiveType(), states);
}