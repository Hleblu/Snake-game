#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

struct Particle {
	sf::Vector2f velocity;
	sf::Vector2f position;
	sf::Color color;
	float lifeTime = 0.f;
};

class ParticleManager : public sf::Drawable
{
	sf::VertexArray vertices;
	std::vector<Particle> particles;
	std::size_t aliveCount = 0;
public:
	ParticleManager(std::size_t count);
	void emit(std::size_t count, sf::Vector2f position, sf::Color color, const float minSpeed, const float maxSpeed);
	void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};