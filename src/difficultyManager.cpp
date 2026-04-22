#include "difficultyManager.hpp"
#include <cmath>

constexpr float PATHING_BIAS = 1.5f;
constexpr int SESSION_COUNT = 10;

void SessionResults::reset()
{
	timeExpected = 0.f;
	timeActual = 0.f;
}

void SessionResults::addExpected(float dt)
{
	timeExpected += dt;
}

void SessionResults::addActual(float dt)
{
	timeActual += dt;
}

float SessionResults::getExpected() const
{
	return timeExpected;
}

float SessionResults::getActual() const
{
	return timeActual;
}

void SessionResults::operator+=(const SessionResults& other)
{
	timeActual += other.timeActual;
	timeExpected += other.timeExpected;
}

std::string SessionResults::getHeader() const
{
	return "SessionResults";
}

void SessionResults::serialize(Archive& archive)
{
	archive
		& timeActual
		& timeExpected;
}

void DifficultyManager::onStart()
{
	currentSession.reset();
}

void DifficultyManager::onEnd()
{
	sessionCounter++;
	unifiedResults += currentSession;

	if (sessionCounter >= SESSION_COUNT) 
	{
		const float timeDifference = unifiedResults.getExpected() - unifiedResults.getActual();
		modificator *= (1 + 0.05f * std::tanh(timeDifference / unifiedResults.getExpected()));
		sessionCounter = 0;
		unifiedResults.reset();
	}
}

void DifficultyManager::updateExpected(const Cell& p, const Cell& g, float occupancy, float delay)
{
	int distance = std::abs(p.x - g.x) + std::abs(p.y - g.y);
	float expected = (distance * delay) * (1 + occupancy) * PATHING_BIAS;
	currentSession.addExpected(expected);
}

void DifficultyManager::updateCurrent(float dt)
{
	currentSession.addActual(dt);
}

float DifficultyManager::getModificator() const
{
	return modificator;
}

std::string DifficultyManager::getHeader() const
{
	return "DifficultyManagement";
}

void DifficultyManager::serialize(Archive& archive)
{
	archive
		& sessionCounter
		& modificator;

	unifiedResults.serialize(archive);
}
