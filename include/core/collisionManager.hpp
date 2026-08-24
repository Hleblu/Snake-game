#pragma once
#include <SFML/System/Vector2.hpp>
#include <array>
#include <vector>
#include <cstdint>

enum class ObjectType : std::uint8_t
{
	NONE = 0,
	SNAKE_HEAD = 1 << 0,
	SNAKE_TAIL = 1 << 1,
	SNAKE_TAIL_GHOST = 1 << 2,
	APPLE = 1 << 3,
	OBSTACLE = 1 << 4
};

inline ObjectType operator&(ObjectType a, ObjectType b) {
	return static_cast<ObjectType>(static_cast<std::uint8_t>(a) & static_cast<std::uint8_t>(b));
}

inline ObjectType operator|(ObjectType a, ObjectType b) {
	return static_cast<ObjectType>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
}

inline ObjectType operator^(ObjectType a, ObjectType b) {
	return static_cast<ObjectType>(static_cast<std::uint8_t>(a) ^ static_cast<std::uint8_t>(b));
}

inline ObjectType operator~(ObjectType a) {
	return static_cast<ObjectType>(~static_cast<std::uint8_t>(a));
}

class CollisionManager
{
	const std::array<sf::Vector2i, 8> neighbourDirections = { {
			{0, 1}, {0, -1}, {1, 0}, {-1, 0},
			{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
		} };
	std::vector<ObjectType> grid{};
	int occupiedCount = 0;
	int width = 0;
	int height = 0;

	inline std::size_t getIndex(const sf::Vector2i& cell) const {
		return cell.y * width + cell.x;
	}
public:
	void init(int rows, int columns);
	void setOccupied(const sf::Vector2i& cell, const ObjectType type);
	void setFree(const sf::Vector2i& cell, const ObjectType type);
	void changeTypes(const sf::Vector2i& cell, const ObjectType oldType, const ObjectType newType);
	void clearMap();
	bool isCellOccupied(const sf::Vector2i& cell) const;
	bool checkCellType(const sf::Vector2i& cell, const ObjectType type) const;
	std::size_t numberOfOccupied() const;
	bool isEmptyAround(const sf::Vector2i& cell, const ObjectType type) const;
	bool isEmptyAround(const sf::Vector2i& cell) const;
	bool isOutOfBorders(const sf::Vector2i& cell) const;
	float getOccupancyRate() const;
};