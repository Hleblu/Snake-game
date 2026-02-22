#pragma once
#include "apple.hpp"
#include "collisionManager.hpp"
#include "flash.hpp"
#include "floatingText.hpp"
#include "obstacle.hpp"
#include "particleManager.hpp"
#include "renderResources.hpp"
#include "snake.hpp"
#include "soundManager.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

class Configuration;

class GameContext
{
public:
    std::uint16_t score = 0;
    float totalTime = 0;

    float deltaTime = 0;
    float updateAccumulator = 0;
    float gameOverTimer = 0;
    float delay = 0;
    
    void reset(float startDelay);
};

class Game 
{
    Configuration* config;
    std::unique_ptr<CollisionManager> collision;
    GameContext context;
    sf::Clock clock;
    sf::Sprite background;
    SoundManager sManager;
    RenderResources resources;
    Snake snake;
    Obstacle obstacle;
    ParticleManager particles;
    std::unique_ptr<Apple> apple;
    Flash flash;
    FloatingText floatingText;
    sf::View gameView;
    sf::Vector2f defCenter;

    enum class State {
        PLAY,
        EXIT,
        PAUSE,
        GAMEOVER
    } state;

    void restoreDefaults();
    float calculateSpeed(std::uint16_t score);
    void initVisuals(sf::RenderWindow& window);
public:
    void start(sf::RenderWindow& window);
    Game(Configuration* config, sf::Font* font);
};