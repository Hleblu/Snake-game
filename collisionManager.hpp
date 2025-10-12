#pragma once
#include "cell.hpp"
#include <array>
#include <unordered_map>
#include <unordered_set>

enum class ObjectType
{
	SNAKE_TAIL,
	SNAKE_HEAD,
	APPLE,
	OBSTACLE
};

class CollisionManager
{
	CollisionManager();

	const std::array<Cell, 8> neighbourDirections = { {
			{0, 1}, {0, -1}, {1, 0}, {-1, 0},
			{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
		} };
	std::unordered_map < Cell, std::unordered_set<ObjectType>, CellHash > collisionMap;
public:
	static CollisionManager& getInstance();
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	void setOccupied(const Cell& cell, const ObjectType type);
	void setFree(const Cell& cell, const ObjectType type);
	void changeTypes(const Cell& cell, const ObjectType oldType, const ObjectType newType);
	void clearMap();
	bool isCellOccupied(const Cell& cell) const;
	bool checkCellType(const Cell& cell, const ObjectType type) const;
	std::size_t numberOfOccupied() const;
	bool isEmptyAround(const Cell& cell, const ObjectType type) const;
	bool isEmptyAround(const Cell& cell) const;
	bool isOutOfBorders(const Cell& cell) const;
};

inline CollisionManager& collisionManager = CollisionManager::getInstance();