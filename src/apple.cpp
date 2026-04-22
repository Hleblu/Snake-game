#include "apple.hpp"
#include "collisionManager.hpp"
#include "configuration.hpp"
#include "randomGenerator.hpp"
#include "snake.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <vector>

Apple::Apple(
    Configuration* config,
    CollisionManager* collision,
    sf::Texture& texture,
    sf::Shader* shader,
    std::unique_ptr<IAppleEffect> eff,
    float spawnTime
) 
    : effect(std::move(eff)),
    sprite(texture),
    shader(shader),
    config(config),
    collision(collision),
    spawnTime(spawnTime)
{
    sprite.setTextureRect(effect->getTextureRect());
    const float scaleX = config->getCellSize() / sprite.getLocalBounds().size.x;
    const float scaleY = config->getCellSize() / sprite.getLocalBounds().size.y;
    sprite.setScale({ scaleX, scaleY });
    sprite.setColor(config->getCurrentTheme().appleColor);

    coords = { -1, -1 };

    generateNewPosition();
}

float Apple::getSpeedBonus() const
{
    return effect->getSpeedBonus();
}

const Cell Apple::getPosition() const 
{
    return coords;
}

void Apple::generateNewPosition()
{
    if (collision->getOccupancyRate() < 1.f) {
        const std::int16_t xMax = config->getColumns();
        const std::int16_t totalCells = config->getTotalSize();
        const int startingIndex = RandomGenerator::getInt(0, totalCells);

        for (std::size_t i = 0; i < totalCells; ++i) {
            const int currentIndex = (startingIndex + i) % totalCells;

            const int x = currentIndex % xMax;
            const int y = currentIndex / xMax;

            const Cell candidate{ x,y };
            if (!collision->isCellOccupied(candidate)) {
                coords = candidate;
                collision->setOccupied(candidate, ObjectType::APPLE);
                break;
            }
        }
    }
    sprite.setPosition(sf::Vector2f(coords.x * config->getCellSize(), coords.y * config->getCellSize()));
}

bool Apple::isEaten() const
{
    return !collision->isOutOfBorders(coords)
        && collision->checkCellType(coords, ObjectType::SNAKE_HEAD);
}

void Apple::applyEffect(Snake& snake) const 
{
    effect->apply(snake);
}

void Apple::updateShader(float currentTime)
{
    alpha = (currentTime - spawnTime) / config->getStartDelay();
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (shader) {
        shader->setUniform("animProgress", alpha);
        states.shader = shader;
    }
    target.draw(sprite, states);
}

Apple::~Apple()
{
    collision->setFree(coords, ObjectType::APPLE);
}

std::unique_ptr<Apple> AppleFactory::createRandomApple(
    Configuration* config,
    CollisionManager* collision,
    sf::Texture& texture,
    sf::Shader* shader,
    float spawnTime
) {
    const int number = RandomGenerator::getInt(0, 99);
    std::unique_ptr<IAppleEffect> effect;

    if (number <= 85) effect = std::make_unique<BasicEffect>();
    else if (number <= 90) effect = std::make_unique<BonusEffect>();
    else if (number <= 95) effect = std::make_unique<HasteEffect>();
    else effect = std::make_unique<SlownessEffect>();

    return std::make_unique<Apple>(config, collision, texture, shader, std::move(effect), spawnTime);
}

constexpr sf::IntRect basicRect({ 0, 0 }, { 64, 64 });
constexpr sf::IntRect bonusRect({ 64, 0 }, { 64, 64 });
constexpr sf::IntRect hasteRect({ 128, 0 }, { 64, 64 });
constexpr sf::IntRect slownessRect({ 192, 0 }, { 64, 64 });

void BasicEffect::apply(Snake& snake) const
{
    snake.grow(1);
}

float BasicEffect::getSpeedBonus() const
{
    return 1.0f;
}

sf::IntRect BasicEffect::getTextureRect() const
{
    return basicRect;
}

void BonusEffect::apply(Snake& snake) const
{
    snake.grow(2);
}

float BonusEffect::getSpeedBonus() const
{
    return 1.0f;
}

sf::IntRect BonusEffect::getTextureRect() const
{
    return bonusRect;
}

void HasteEffect::apply(Snake& snake) const
{
    snake.grow(1);
}

float HasteEffect::getSpeedBonus() const
{
    return 0.8f;
}

sf::IntRect HasteEffect::getTextureRect() const
{
    return hasteRect;
}

void SlownessEffect::apply(Snake& snake) const
{
    snake.grow(1);
}

float SlownessEffect::getSpeedBonus() const
{
    return 1.2f;
}

sf::IntRect SlownessEffect::getTextureRect() const
{
    return slownessRect;
}
