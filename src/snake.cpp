#include "collisionManager.hpp"
#include "configuration.hpp"
#include "randomGenerator.hpp"
#include "snake.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

constexpr std::size_t VERTEX_PER_TRIANGLE = 6;

Snake::Snake(Configuration* config, CollisionManager* collision, sf::Shader* shader) 
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
    const Cell& head = segments.front();

    const float x = head.x * config->getCellSize() + config->getCellSize() / 2.f;
    const float y = head.y * config->getCellSize() + config->getCellSize() / 2.f;

    return sf::Vector2f(x, y);
}

Direction Snake::getDirection() const
{
    return dir;
}

void Snake::restoreDefaultValues()
{
    const std::int16_t centerX = config->getColumns() / 2;
    const std::int16_t centerY = config->getRows() / 2;

    segments.clear();
    for (int i = 0; i < config->getSnakeDefSize(); ++i)
        segments.push_back({ centerX - i, centerY });

    lastSegments = segments;

    for (auto& seg : segments)
        collision->setOccupied(seg, ObjectType::SNAKE_TAIL);
    collision->changeTypes(segments.front(), ObjectType::SNAKE_TAIL, ObjectType::SNAKE_HEAD);

    dir = Direction::Val::NONE;
    prevDir = Direction::Val::NONE;
    nextDir = Direction::Val:: NONE;

    hasNotMovedYet = true;
    collisionPending = 0;

    vertices.clear();
    vertices.resize(config->getRows() * config->getColumns() * VERTEX_PER_TRIANGLE);
    jointVertices.clear();

    updateVertices();
	updateTexCoords();

    if (shader) {
        shader->setUniform("startColor", sf::Glsl::Vec4(config->getCurrentTheme().snakeColor));
        shader->setUniform("endColor", sf::Glsl::Vec4(config->getCurrentTheme().snakeColorEnd));
        shader->setUniform("fadeDuration", config->gameOverDelay);
        shader->setUniform("fadeStartTime", -1.f);
    }
}

bool Snake::hasCollided() const
{
    const Cell& head = segments.front();
    const ObjectType danger = ObjectType::SNAKE_TAIL | ObjectType::OBSTACLE;
    return collision->checkCellType(head, danger);
}

void Snake::triggerDeath(float currentTime)
{
    if (shader)
        shader->setUniform("fadeStartTime", currentTime);
}

bool Snake::isWaitingForFirstMove() const
{
    return hasNotMovedYet;
}

void Snake::grow(int size)
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
        collision->setFree(lastSegments.back(), ObjectType::SNAKE_TAIL_GHOST);
        collision->changeTypes(segments.back(), ObjectType::SNAKE_TAIL, ObjectType::SNAKE_TAIL_GHOST);
    }
    else collisionPending -= 1;

    if (!hasNotMovedYet) { 
        lastSegments.pop_back();
        lastSegments.emplace_front(segments.front());
    }
    else hasNotMovedYet = false;
    
    segments.pop_back();
    segments.emplace_front(segments.front());
    collision->changeTypes(segments.front(), ObjectType::SNAKE_HEAD, ObjectType::SNAKE_TAIL);

    const sf::Vector2i offset = dir.getVector();
    segments[0] += Cell({ offset.x, offset.y });

    collision->setOccupied(segments.front(), ObjectType::SNAKE_HEAD);
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

Cell Snake::getHead() const
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

void Snake::updateVertices(float dt)
{
    for (size_t i = 0; i < segments.size(); ++i) {
        const float posX = (lastSegments[i].x + (segments[i].x - lastSegments[i].x) * dt) * config->getCellSize();
        const float posY = (lastSegments[i].y + (segments[i].y - lastSegments[i].y) * dt) * config->getCellSize();
		const float posXEnd = posX + config->getCellSize();
		const float posYEnd = posY + config->getCellSize();

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
        const Cell nextSeg = lastSegments[i];

        if (prevSeg.x != nextSeg.x && prevSeg.y != nextSeg.y) {
            const float posX = thisSeg.x * config->getCellSize();
            const float posY = thisSeg.y * config->getCellSize();
            const float posXEnd = posX + config->getCellSize();
            const float posYEnd = posY + config->getCellSize();
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
    if (shader) {
        shader->setUniform("currentTime", shaderTime);
        states.shader = shader;
    }

    target.draw(jointVertices, states);
    target.draw(&vertices[0], getSize() * VERTEX_PER_TRIANGLE, vertices.getPrimitiveType(), states);
}