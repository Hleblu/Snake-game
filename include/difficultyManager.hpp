#pragma once
#include "serializable.hpp"
#include "cell.hpp"

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
};

class DifficultyManager : public Serializable
{
private:
	SessionResults unifiedResults;
	SessionResults currentSession;
	int sessionCounter = 0;
	float modificator = 1.f;

public:
	void onStart();
	void onEnd();
	void updateExpected(const Cell& p, const Cell& g, float occupancy, float delay);
	void updateCurrent(float dt);
	float getModificator() const;

	std::string getHeader() const override;
	void serialize(Archive& archive) override;
};