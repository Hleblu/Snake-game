#include "collisionManager.h"
CollisionManager* CollisionManager::instance = nullptr;
CollisionManager::CollisionManager() {}
CollisionManager* CollisionManager::getInstance() {
	if (instance == nullptr) {
		instance = new CollisionManager();
	}
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

int CollisionManager::numberOfOccupied() const {
	return collisionMap.size();
}