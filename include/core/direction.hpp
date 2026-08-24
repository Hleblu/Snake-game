#pragma once
#include <SFML/System/Vector2.hpp>

class Direction
{
public:
	enum class Val
	{
		NONE = -1,
		LEFT = 1,
		UP = 2,
		RIGHT = 3,
		DOWN = 4
	} value;

	operator Val() const { return value; }
	bool operator==(Val other) const { return value == other; }
	void operator=(Val other) { value = other; }

	sf::Vector2i getVector() const
	{
		switch (value)
		{
			case Direction::Val::LEFT: return { -1, 0 };
			case Direction::Val::UP: return { 0, -1 };
			case Direction::Val::RIGHT: return { 1, 0 };
			case Direction::Val::DOWN: return { 0, 1 };
			default: return { 0, 0 };
		}
	}

	static sf::Vector2i getVector(Direction::Val value)
	{
		switch (value)
		{
			case Direction::Val::LEFT: return { -1, 0 };
			case Direction::Val::UP: return { 0, -1 };
			case Direction::Val::RIGHT: return { 1, 0 };
			case Direction::Val::DOWN: return { 0, 1 };
			default: return { 0, 0 };
		}
	}

	bool isOpposite(Direction other) const
	{
		return static_cast<int>(this->value) % 2 == static_cast<int>(other.value) % 2;
	}
};
