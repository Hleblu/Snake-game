#pragma once
#include "serializable.hpp"
#include <SFML/System/Vector2.hpp>

class SessionResults : public Serializable
{
private:
	float timeExpected = 0.f;
	float timeActual = 0.f;

public:
	void reset();
	void addExpected(float dt);
	void addActual(float dt);
	float getExpected() const;
	float getActual() const;

	void operator+=(const SessionResults& other);

	std::string getHeader() const override;
	void serialize(Archive& archive) override;
	void setDefaults() override;
};

class DifficultyManager : public Serializable
{
private:
	SessionResults unifiedResults;
	SessionResults currentSession;
	int sessionCounter = 0;
	float modifier = 1.f;

public:
	void onStart();
	void onEnd();
	void updateExpected(const sf::Vector2i& p, const sf::Vector2i& g, float occupancy, float delay);
	void updateCurrent(float dt);
	float getModifier() const;

	std::string getHeader() const override;
	void serialize(Archive& archive) override;
	void setDefaults() override;
};