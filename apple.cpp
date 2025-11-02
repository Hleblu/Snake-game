#include "apple.hpp"
#include "collisionManager.hpp"
#include "configuration.hpp"
#include "randomGenerator.hpp"
#include "snake.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

Apple::Apple(std::unique_ptr<IAppleEffect> eff, sf::Texture& texture) 
    : effect(std::move(eff)), sprite(texture)
{
    sprite.setTextureRect(effect->getTextureRect());
    const float scaleX = config.getCellSize() / sprite.getLocalBounds().size.x;
    const float scaleY = config.getCellSize() / sprite.getLocalBounds().size.y;
    sprite.setScale({ scaleX, scaleY });
    sprite.setColor(config.getAppleColor());

    generateNewPosition();
}

float Apple::getSpeedBonus() const
{
    return effect->getSpeedBonus();
}

void Apple::generateNewPosition()
{
    collisionManager.setFree(coords, ObjectType::APPLE);
    coords = { -1, -1 };
    if (collisionManager.numberOfOccupied() >= config.getRows() * config.getColumns()) return;

    const std::int16_t xMax = config.getRows() - 1;
    const std::int16_t yMax = config.getColumns() - 1;
    std::vector<Cell> freeCells{};
    for (int x = 0; x <= xMax; ++x) {
        for (int y = 0; y <= yMax; ++y) {
            if (!collisionManager.isCellOccupied({ x,y }))
                freeCells.emplace_back( x,y );
        }
    }

    if (freeCells.empty()) return;

    const int index = RandomGenerator::getInt(0, freeCells.size() - 1);
    coords = freeCells[index];

    sprite.setPosition(sf::Vector2f(coords.x * config.getCellSize(), coords.y * config.getCellSize()));
    collisionManager.setOccupied(coords, ObjectType::APPLE);
}

bool Apple::isEaten() const
{
    return collisionManager.checkCellType(coords, ObjectType::SNAKE_HEAD);
}

void Apple::applyEffect(Snake& snake) const 
{
    effect->apply(snake);
}

void Apple::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}

std::unique_ptr<Apple> AppleFactory::createRandomApple(sf::Texture& texture) {
    const int number = RandomGenerator::getInt(0, 100);
    std::unique_ptr<IAppleEffect> effect;

    if (number <= 85) effect = std::make_unique<BasicEffect>();
    else if (number <= 90) effect = std::make_unique<BonusEffect>();
    else if (number <= 95) effect = std::make_unique<HasteEffect>();
    else effect = std::make_unique<SlownessEffect>();

    return std::make_unique<Apple>(std::move(effect), texture);
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
