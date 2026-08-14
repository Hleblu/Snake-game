#include "collisionManager.hpp"
#include "configuration.hpp"
#include "obstacle.hpp"
#include "randomGenerator.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

Obstacle::Obstacle(Configuration& config, CollisionManager& collision, sf::Shader* shader)
    : config(config),
    collision(collision),
    shader(shader)
{
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    restoreDefaultValues();
};

void Obstacle::updateShader(float currentTime)
{
    shaderTime = currentTime;
}

void Obstacle::generateNewPosition(float creationTime) {
    if (collision.getOccupancyRate() > 0.9f) return;

    const std::int16_t xMax = config.getColumns() - 1;
    const std::int16_t yMax = config.getRows() - 1;
    const std::int16_t totalCells = xMax * yMax;
    const int startingIndex = RandomGenerator::getInt(0, totalCells);

    for (std::size_t i = 0; i < totalCells; ++i) {
        const int currentIndex = (startingIndex + i) % totalCells;

        const int x = (currentIndex % xMax) + 1;
        const int y = (currentIndex / xMax) + 1;

        const sf::Vector2i candidate{ x, y };
        if (!collision.isCellOccupied(candidate) && collision.isEmptyAround(candidate)) {
            collision.setOccupied(candidate, ObjectType::OBSTACLE);
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
    const float cellSize = config.getCellSize();
    const sf::Vector2f cellSizeVector = { cellSize, cellSize };

    const sf::Vector2f coord = sf::Vector2f(coords.back());
    const sf::Vector2f pos = coord * cellSize;
    const sf::Vector2f posEnd = pos + cellSizeVector;

    const auto color = config.getCurrentTheme().obstacleColor;

    sf::Vector2f texCoord = { creationTime, 0.f };

    vertices.append(sf::Vertex{ { pos }, color, texCoord });
    vertices.append(sf::Vertex{ { posEnd.x, pos.y }, color, texCoord });
    vertices.append(sf::Vertex{ { posEnd }, color, texCoord });
    vertices.append(sf::Vertex{ { posEnd }, color, texCoord });
    vertices.append(sf::Vertex{ { pos.x, posEnd.y }, color, texCoord });
    vertices.append(sf::Vertex{ { pos }, color, texCoord });
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!config.areObstaclesEnabled()) return;
    if (shader) {
        shader->setUniform("duration", config.getStartDelay());
        shader->setUniform("currentTime", shaderTime);
        states.shader = shader;
    }
    target.draw(vertices, states);
}