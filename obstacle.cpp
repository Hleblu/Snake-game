#include "collisionManager.hpp"
#include "configuration.hpp"
#include "obstacle.hpp"
#include "randomGenerator.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Obstacle::Obstacle() {
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    restoreDefaultValues();
};

void Obstacle::generateNewPosition(float creationTime) {
    if (collisionManager.numberOfOccupied() >= config.getRows() * config.getColumns() - 9) return;

    const std::int16_t xMax = config.getRows() - 2;
    const std::int16_t yMax = config.getColumns() - 2;
    std::vector<Cell> freeCells{};
    for (int x = 1; x <= xMax; ++x) {
        for (int y = 1; y <= yMax; ++y) {
            if (!collisionManager.isCellOccupied({ x,y })
                && collisionManager.isEmptyAround({ x, y }))
            {
                freeCells.emplace_back(x, y);
            }
        }
    }

    if (freeCells.empty()) return;

    Cell coord;
    const int index = RandomGenerator::getInt(0, freeCells.size() - 1);
    coord = freeCells[index];

    collisionManager.setOccupied(coord, ObjectType::OBSTACLE);
    coords.push_back(coord);
    updateVertices(creationTime);
}

void Obstacle::restoreDefaultValues()
{
    vertices.clear();
    coords.clear();
}

void Obstacle::updateVertices(float creationTime) {
    const Cell& coord = coords.back();
    const float posX = coord.x * config.getCellSize();
    const float posY = coord.y * config.getCellSize();
    const float posXEnd = posX + config.getCellSize();
    const float posYEnd = posY + config.getCellSize();
    const auto color = config.getObstacleColor();

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
    target.draw(vertices, states);
}