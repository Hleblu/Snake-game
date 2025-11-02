#include "collisionManager.hpp"
#include "configuration.hpp"

CollisionManager::CollisionManager() {}
CollisionManager& CollisionManager::getInstance() {
	static CollisionManager instance;
	return instance;
}

void CollisionManager::setOccupied(const Cell& cell, const ObjectType type) {
	collisionMap[cell].insert(type);
}

void CollisionManager::setFree(const Cell& cell, const ObjectType type) {
	auto it = collisionMap.find(cell);
	if (it != collisionMap.end()) {
		it->second.erase(type);

		if (it->second.empty())
			collisionMap.erase(it);
	}
}

void CollisionManager::clearMap() {
	collisionMap.clear();
}

bool CollisionManager::isCellOccupied(const Cell& cell) const {
	auto it = collisionMap.find(cell);
	return it != collisionMap.end() && !it->second.empty();
}

bool CollisionManager::checkCellType(const Cell& cell, const ObjectType type) const {
	auto it = collisionMap.find(cell);
	return it != collisionMap.end() && it->second.count(type) != 0;
}

void CollisionManager::changeTypes(const Cell& cell, const ObjectType oldType, const ObjectType newType) {
	if (collisionMap[cell].count(oldType) != 0) {
		collisionMap[cell].erase(oldType);
		collisionMap[cell].insert(newType);
	}
}

std::size_t CollisionManager::numberOfOccupied() const {
	return collisionMap.size();
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
	return cell.x < 0 || cell.x > config.getRows() - 1 || cell.y < 0 || cell.y > config.getColumns() - 1;
}