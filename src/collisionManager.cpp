#include "collisionManager.hpp"
void CollisionManager::init(int rows, int columns)
{
	clearMap();
	if (columns != width || rows != height) {
		width = columns;
		height = rows;
		grid.resize(width * height, ObjectType::NONE);
	}
}

void CollisionManager::setOccupied(const Cell& cell, const ObjectType type) {
	if (isOutOfBorders(cell))
		return;

	auto index = getIndex(cell);

	if (grid[index] == ObjectType::NONE)
		occupiedCount++;

	grid[index] = grid[index] | type;
}

void CollisionManager::setFree(const Cell& cell, const ObjectType type) {
	if (isOutOfBorders(cell))
		return;

	auto index = getIndex(cell);

	grid[index] = grid[index] & (~type);

	if (grid[index] == ObjectType::NONE)
		occupiedCount--;
}

void CollisionManager::clearMap() {
	std::fill(grid.begin(), grid.end(), ObjectType::NONE);
	occupiedCount = 0;
}

bool CollisionManager::isCellOccupied(const Cell& cell) const {
	if (isOutOfBorders(cell))
		return true;

	auto index = getIndex(cell);

	return grid[index] != ObjectType::NONE;
}

bool CollisionManager::checkCellType(const Cell& cell, const ObjectType type) const {
	if (isOutOfBorders(cell))
		return true;

	auto index = getIndex(cell);

	return (grid[index] & type) != ObjectType::NONE;
}

void CollisionManager::changeTypes(const Cell& cell, const ObjectType oldType, const ObjectType newType) {
	if (isOutOfBorders(cell)) return;

	auto index = getIndex(cell);

	grid[index] = grid[index] & (~oldType);
	grid[index] = grid[index] | newType;
}

std::size_t CollisionManager::numberOfOccupied() const {
	return occupiedCount;
}

bool CollisionManager::isEmptyAround(const Cell& cell, const ObjectType type) const {
	for (const auto& dir : neighbourDirections) {
		const Cell suspectedCell = { cell.x + dir.x, cell.y + dir.y };
		if (checkCellType(suspectedCell, type))
			return false;
	}
	return true;
}

bool CollisionManager::isEmptyAround(const Cell& cell) const {
	for (const auto& dir : neighbourDirections) {
		const Cell suspectedCell = { cell.x + dir.x, cell.y + dir.y };
		if (isCellOccupied(suspectedCell))
			return false;
	}
	return true;
}

bool CollisionManager::isOutOfBorders(const Cell& cell) const {
	return cell.x < 0 || cell.x >= width || cell.y < 0 || cell.y >= height;
}

float CollisionManager::getOccupancyRate() const
{
	const int totalCells = width * height;
	const float occupied = numberOfOccupied() / totalCells;

	return occupied;
}