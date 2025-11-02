#include "particleManager.hpp"
#include "randomGenerator.hpp"
#include <cmath>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

constexpr float MAX_LIFE_TIME = 1.5f;
constexpr float MIN_LIFE_TIME = 0.5f;

ParticleManager::ParticleManager(std::size_t count)
	: particles(count), vertices(sf::PrimitiveType::Points, count) { }

void ParticleManager::emit
(
	std::size_t count,
	sf::Vector2f position,
	sf::Color color,
	const float minSpeed,
	const float maxSpeed
)
{
	for (std::size_t i = 0; i < count; ++i) {
		if (aliveCount >= particles.size())
			break;
		Particle& p = particles[aliveCount];

		p.color = color;
		p.position = position;
		p.lifeTime = RandomGenerator::getFloat(MIN_LIFE_TIME, MAX_LIFE_TIME);

		const float angle = RandomGenerator::getFloat(0.f, 2.f * 3.14159f);
		const float speed = RandomGenerator::getFloat(minSpeed, maxSpeed);
		p.velocity = { std::cos(angle) * speed, std::sin(angle) * speed };

		vertices[aliveCount].position = p.position;
		vertices[aliveCount].color = p.color;

		aliveCount++;
	}
}

void ParticleManager::update(float dt)
{
	for (std::size_t i = 0; i < aliveCount;) {
		Particle& p = particles[i];
		p.lifeTime -= dt;

		if (p.lifeTime <= 0.f) {
			aliveCount--;
			p.color.a = 0;

			std::swap(particles[i], particles[aliveCount]);
			std::swap(vertices[i], vertices[aliveCount]);

			continue;
		}

		p.velocity *= 0.99f;

		p.position += p.velocity * dt;
		vertices[i].position = p.position;
		vertices[i].color = p.color;

		const float lifePercent = p.lifeTime / MAX_LIFE_TIME;
		vertices[i].color.a = static_cast<std::uint8_t>(lifePercent * 255.f);

		i++;
	}
}


void ParticleManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(&vertices[0], aliveCount, vertices.getPrimitiveType(), states);
}

