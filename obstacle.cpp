#include "collisionManager.hpp"
#include "configuration.hpp"
#include "obstacle.hpp"
#include "randomGenerator.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

Obstacle::Obstacle(Configuration* config, CollisionManager* collision, sf::Shader* shader)
    : config(config),
    collision(collision),
    shader(shader)
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    restoreDefaultValues();
};

void Obstacle::updateShader(float currentTime)
{
    time = currentTime;
}

void Obstacle::generateNewPosition(float creationTime) {
    if (!collision->isOccupancyBelow(90)) return;

    const std::int16_t xMax = config->getColumns() - 1;
    const std::int16_t yMax = config->getRows() - 1;
    const std::int16_t totalCells = xMax * yMax;
    const int startingIndex = RandomGenerator::getInt(0, totalCells);

    for (std::size_t i = 0; i < totalCells; ++i) {
        const int currentIndex = (startingIndex + i) % totalCells;

        const int x = (currentIndex % xMax) + 1;
        const int y = (currentIndex / xMax) + 1;

        const Cell candidate{ x, y };
        if (!collision->isCellOccupied(candidate) && collision->isEmptyAround(candidate)) {
            collision->setOccupied(candidate, ObjectType::OBSTACLE);
            coords.push_back(candidate);
            updateVertices(creationTime);
            break;
        }
    }
}

void Obstacle::restoreDefaultValues()
{
    vertices.clear();
    coords.clear();
}

void Obstacle::updateVertices(float creationTime) {
    const Cell& coord = coords.back();
    const float posX = coord.x * config->getCellSize();
    const float posY = coord.y * config->getCellSize();
    const float posXEnd = posX + config->getCellSize();
    const float posYEnd = posY + config->getCellSize();
    const auto color = config->getCurrentTheme().obstacleColor;

    sf::Vector2f texCoord = { creationTime, 0.f };

    vertices.append(sf::Vertex{ { posX, posY }, color, texCoord });
    vertices.append(sf::Vertex{ { posXEnd, posY }, color, texCoord });
    vertices.append(sf::Vertex{ { posXEnd, posYEnd }, color, texCoord });
    vertices.append(sf::Vertex{ { posXEnd, posYEnd }, color, texCoord });
    vertices.append(sf::Vertex{ { posX, posYEnd }, color, texCoord });
    vertices.append(sf::Vertex{ { posX, posY }, color, texCoord });
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!config->areObstaclesEnabled()) return;
    if (shader) {
        shader->setUniform("duration", config->getStartDelay());
        shader->setUniform("currentTime", time);
        states.shader = shader;
    }
    target.draw(vertices, states);
}