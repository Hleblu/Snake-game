#include "particleManager.hpp"
#include "randomGenerator.hpp"
#include <cmath>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

constexpr float MAX_LIFE_TIME = 0.8f;
constexpr float MIN_LIFE_TIME = 0.4f;
constexpr int VERTICES_COUNT = 6;
constexpr float GRAVITY = 800.f;

ParticleManager::ParticleManager(std::size_t count)
	: particles(count), vertices(sf::PrimitiveType::Triangles, count * VERTICES_COUNT) { }

namespace {
	void updateVertices(sf::VertexArray& vertices, int index, sf::Vector2f position, float size, const sf::Color& color)
	{
		const std::size_t vIndex = index * VERTICES_COUNT;
		const float halfSize = size /= 2.f;

		vertices[vIndex].position = { position.x - halfSize, position.y - halfSize };
		vertices[vIndex + 1].position = { position.x + halfSize, position.y - halfSize };
		vertices[vIndex + 2].position = { position.x - halfSize, position.y + halfSize };
		vertices[vIndex + 3].position = { position.x + halfSize, position.y - halfSize };
		vertices[vIndex + 4].position = { position.x + halfSize, position.y + halfSize };
		vertices[vIndex + 5].position = { position.x - halfSize, position.y + halfSize };

		vertices[vIndex].color = color;
        vertices[vIndex + 1].color = color;
        vertices[vIndex + 2].color = color;
        vertices[vIndex + 3].color = color;
        vertices[vIndex + 4].color = color;
        vertices[vIndex + 5].color = color;
	}

	sf::Color applyBrightness(const sf::Color& color, float factor) 
	{
		return sf::Color
		(
			static_cast<std::uint8_t>(color.r * factor),
			static_cast<std::uint8_t>(color.g * factor),
			static_cast<std::uint8_t>(color.b * factor),
			color.a
		);
	}
}

void ParticleManager::emit
(
	std::size_t count,
	sf::Vector2f position,
	float size,
	sf::Color color,
	float minSpeed,
	float maxSpeed
)
{
	for (std::size_t i = 0; i < count; ++i) {
		if (aliveCount >= particles.size())
			break;
		Particle& p = particles[aliveCount];

		float brightnessFactor = RandomGenerator::getFloat(0.8f, 1.0f);
		sf::Color particleColor = applyBrightness(color, brightnessFactor);

		float sizeFactor = RandomGenerator::getFloat(0.8f, 1.2f);
		float particleSize = size * sizeFactor;

		p.color = particleColor;
		p.position = position;
		p.size = particleSize;
		p.lifeTime = RandomGenerator::getFloat(MIN_LIFE_TIME, MAX_LIFE_TIME);

		const float angle = RandomGenerator::getFloat(0.f, 2.f * 3.14159f);
		const float speed = RandomGenerator::getFloat(minSpeed, maxSpeed);
		p.velocity = { std::cos(angle) * speed, std::sin(angle) * speed };
		p.velocity.y -= speed * 1.2f;

		updateVertices(vertices, aliveCount, p.position, p.size, p.color);

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

			std::swap(particles[i], particles[aliveCount]);

			std::size_t thisIndex = i * VERTICES_COUNT;
			std::size_t lastAliveIndex = aliveCount * VERTICES_COUNT;
			
			for (std::size_t j = 0; j < VERTICES_COUNT; ++j)
			{
				vertices[thisIndex + j] = vertices[lastAliveIndex + j];
			}

			continue;
		}
		p.velocity.y += GRAVITY * dt;
		p.position += p.velocity * dt;

		//const float lifePercent = p.lifeTime / MAX_LIFE_TIME;
		//p.color.a = static_cast<std::uint8_t>(lifePercent * 255.f);

		updateVertices(vertices, i, p.position, p.size, p.color);

		i++;
	}
}

void ParticleManager::clearParticles()
{
	aliveCount = 0;
}


void ParticleManager::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(&vertices[0], aliveCount * 6, vertices.getPrimitiveType(), states);
}

