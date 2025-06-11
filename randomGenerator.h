#pragma once
#include <random>

class RandomGenerator
{
public:
	static std::mt19937& getGenerator() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return gen;
	}

	static int getInt(const int min, const int max) {
		std::uniform_int_distribution<> dist(min, max);
		return dist(getGenerator());
	}
};