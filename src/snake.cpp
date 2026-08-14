#include "collisionManager.hpp"
#include "configuration.hpp"
#include "randomGenerator.hpp"
#include "snake.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

namespace {
    constexpr std::size_t VERTICES_PER_QUAD = 6;
}

Snake::Snake(Configuration& config, CollisionManager& collision, sf::Shader* shader) 
    : config(config),
    collision(collision),
    shader(shader)
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    jointVertices.setPrimitiveType(sf::PrimitiveType::Triangles);

    restoreDefaultValues();
}

sf::Vector2f Snake::getHeadCenter() const
{
    const sf::Vector2f head{ segments.front() };
    const float cellSize = config.getCellSize();
    const sf::Vector2f cellSizeVector  = { cellSize, cellSize };

    return head * cellSize + cellSizeVector / 2.f;
}

Direction Snake::getDirection() const
{
    return dir;
}

void Snake::restoreDefaultValues()
{
    const std::int16_t centerX = config.getColumns() / 2;
    const std::int16_t centerY = config.getRows() / 2;

    segments.clear();
    for (int i = 0; i < config.getSnakeDefSize(); ++i)
        segments.push_back({ centerX - i, centerY });

    lastSegments = segments;

    for (auto& seg : segments)
        collision.setOccupied(seg, ObjectType::SNAKE_TAIL);
    collision.changeTypes(segments.front(), ObjectType::SNAKE_TAIL, ObjectType::SNAKE_HEAD);

    dir = Direction::Val::NONE;
    prevDir = Direction::Val::NONE;
    nextDir = Direction::Val:: NONE;

    hasNotMovedYet = true;
    collisionPending = 0;

    vertices.clear();
    vertices.resize(config.getRows() * config.getColumns() * VERTICES_PER_QUAD);
    jointVertices.clear();

    updateVertices();
	updateTexCoords();

    if (shader) {
        shader->setUniform("startColor", sf::Glsl::Vec4(config.getCurrentTheme().snakeColor));
        shader->setUniform("endColor", sf::Glsl::Vec4(config.getCurrentTheme().snakeColorEnd));
        shader->setUniform("fadeDuration", config.getSnakeFadeDuration());
        shader->setUniform("fadeStartTime", -config.getSnakeFadeDuration());
    }
}

bool Snake::hasCollided() const
{
    const sf::Vector2i& head = segments.front();
    const ObjectType danger = ObjectType::SNAKE_TAIL | ObjectType::OBSTACLE;
    return collision.checkCellType(head, danger);
}

void Snake::triggerDeath(float currentTime)
{
    if (shader)
        shader->setUniform("fadeStartTime", currentTime + config.getSnakeFadeDelay());
}

bool Snake::isWaitingForFirstMove() const
{
    return hasNotMovedYet;
}

void Snake::grow(unsigned int size)
{
    collisionPending += size;
    const size_t newSize = segments.size() + size;
    segments.resize(newSize, lastSegments.back());
    lastSegments.resize(newSize, lastSegments.back());
	updateTexCoords();
}

void Snake::move()
{
    if (dir == Direction::Val::NONE) return;

    if (collisionPending == 0) {
        collision.setFree(lastSegments.back(), ObjectType::SNAKE_TAIL_GHOST);
        collision.changeTypes(segments.back(), ObjectType::SNAKE_TAIL, ObjectType::SNAKE_TAIL_GHOST);
    }
    else collisionPending -= 1;

    if (!hasNotMovedYet) { 
        lastSegments.pop_back();
        lastSegments.emplace_front(segments.front());
    }
    else hasNotMovedYet = false;
    
    segments.pop_back();
    segments.emplace_front(segments.front());
    collision.changeTypes(segments.front(), ObjectType::SNAKE_HEAD, ObjectType::SNAKE_TAIL);

    const sf::Vector2i offset = dir.getVector();
    segments[0] += offset;

    collision.setOccupied(segments.front(), ObjectType::SNAKE_HEAD);
    prevDir = dir;

    updateJointVertices();
}

void Snake::setNextDirection(Direction::Val dir)
{
    nextDir = dir;
}

bool Snake::updateDirection()
{
    bool canUpdate = canUpdateDirection();
    if (canUpdate)
        dir = nextDir;
    return canUpdate;
}

bool Snake::canUpdateDirection() const
{
    return nextDir != dir && !prevDir.isOpposite(nextDir);
}

size_t Snake::getSize() const
{
    return segments.size();
}

sf::Vector2i Snake::getHead() const
{
    return segments[0];
}

void Snake::updateShader(float currentTime)
{
    shaderTime = currentTime;
}

void Snake::updateTexCoords()
{
    if (segments.size() <= 1) return;
	for (size_t i = 0; i < segments.size(); ++i) {
        const float normalizedPosition = static_cast<float>(i) / (segments.size() - 1);
        sf::Vertex* triangles = &vertices[i * VERTICES_PER_QUAD];
        const sf::Vector2f texCoord = { 0, normalizedPosition };
        triangles[0].texCoords = texCoord;
        triangles[1].texCoords = texCoord;
        triangles[2].texCoords = texCoord;
        triangles[3].texCoords = texCoord;
        triangles[4].texCoords = texCoord;
        triangles[5].texCoords = texCoord;
	}
}

void Snake::updateVertices(float dt)
{
    const float cellSize = static_cast<float>(config.getCellSize());
    const sf::Vector2f cellSizeVector = { cellSize, cellSize };

    for (size_t i = 0; i < segments.size(); ++i) {

        const sf::Vector2f lastPos{ lastSegments[i] };
        const sf::Vector2f currPos{ segments[i] };

        const sf::Vector2f lerpedPos = (lastPos + (currPos - lastPos) * dt) * cellSize;
        const sf::Vector2f lerpedPosEnd = lerpedPos + cellSizeVector;

        sf::Vertex* triangles = &vertices[i * VERTICES_PER_QUAD];
        triangles[0].position = { lerpedPos };
        triangles[1].position = { lerpedPosEnd.x, lerpedPos.y };
        triangles[2].position = { lerpedPosEnd };
        triangles[3].position = { lerpedPosEnd };
        triangles[4].position = { lerpedPos.x, lerpedPosEnd.y };
        triangles[5].position = { lerpedPos };
    }
}

void Snake::updateJointVertices()
{
    jointVertices.clear();

    const float cellSize = static_cast<float>(config.getCellSize());
    const sf::Vector2f cellSizeVector = { cellSize, cellSize };

    for (size_t i = 1; i < getSize(); ++i) {
        const sf::Vector2f prevSeg{ segments[i - 1] };
        const sf::Vector2f thisSeg{ segments[i] };
        const sf::Vector2f nextSeg{ lastSegments[i] };

        if (prevSeg.x != nextSeg.x && prevSeg.y != nextSeg.y) {
            const sf::Vector2f pos = thisSeg * cellSize;
            const sf::Vector2f posEnd = pos + cellSizeVector;

            const float normalizedPosition = static_cast<float>(i) / (segments.size() - 1);
            const sf::Vector2f texCoord = { 0, normalizedPosition };

            jointVertices.append({ { pos }, sf::Color::White, texCoord });
            jointVertices.append({ { posEnd.x, pos.y }, sf::Color::White, texCoord });
            jointVertices.append({ { posEnd }, sf::Color::White, texCoord });
            jointVertices.append({ { posEnd}, sf::Color::White, texCoord });
            jointVertices.append({ { pos.x, posEnd.y }, sf::Color::White, texCoord });
            jointVertices.append({ { pos }, sf::Color::White, texCoord });
        }
    }
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (shader) {
        shader->setUniform("currentTime", shaderTime);
        states.shader = shader;
    }

    target.draw(jointVertices, states);
    target.draw(&vertices[0], getSize() * VERTICES_PER_QUAD, vertices.getPrimitiveType(), states);
}