#include "obstacle.h"

Obstacle::Obstacle() {
    vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    restoreDefaultValues();
};

void Obstacle::generateNewPosition() {
    if (collisionManager->numberOfOccupied() >= 0.7f * config->rows * config->columns) return;

    Cell coord;
    const short int xMax = config->rows - 2;
    const short int yMax = config->columns - 2;
    short int attempts = 0; 
    const short int maxAttempts = (config->rows * config->columns) / 10;

    do {
        attempts += 1;
        coord.x = RandomGenerator::getInt(1, xMax);
        coord.y = RandomGenerator::getInt(1, yMax);

        if (attempts >= maxAttempts)
            return;
    } while (collisionManager->isCellOccupied(coord) || !collisionManager->isEmptyAround(coord));

    collisionManager->setOccupied(coord, OBSTACLE);
    coords.push_back(coord);
    updateVertexArray();
}

void Obstacle::restoreDefaultValues()
{
    vertices.clear();
    coords.clear();
}

void Obstacle::updateVertexArray() {
    const Cell& coord = coords.back();
    const float posX = coord.x * config->size;
    const float posY = coord.y * config->size;
    const float posXEnd = posX + config->size;
    const float posYEnd = posY + config->size;
    const auto color = config->currentTheme->obstacleColor;

    vertices.append(sf::Vertex{ { posX, posY }, color });
    vertices.append(sf::Vertex{ { posXEnd, posY }, color });
    vertices.append(sf::Vertex{ { posXEnd, posYEnd }, color });
    vertices.append(sf::Vertex{ { posXEnd, posYEnd }, color });
    vertices.append(sf::Vertex{ { posX, posYEnd }, color });
    vertices.append(sf::Vertex{ { posX, posY }, color });
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(vertices, states);
}