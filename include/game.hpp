#pragma once
#include "apple.hpp"
#include "checkeredBG.hpp"
#include "collisionManager.hpp"
#include "difficultyManager.hpp"
#include "flash.hpp"
#include "floatingText.hpp"
#include "obstacle.hpp"
#include "particleManager.hpp"
#include "renderResources.hpp"
#include "snake.hpp"
#include "soundManager.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>

class Configuration;

class GameContext
{
public:
    std::uint16_t score = 0;
    float shaderTime = 0.f;
    float deltaTime = 0.f;
    float updateAccumulator = 0.f;
    float gameOverTimer = 0.f;
    float updateDelay = 0.f;
    bool gameStarted = false;
    
    void reset(float startDelay);
};

class Game
{
    Configuration& config;
    CollisionManager collision;
    RenderResources resources;
    GameContext context;
    sf::Clock clock;
    ChekeredBG background;
    SoundManager sounds;
    Snake snake;
    Obstacle obstacle;
    ParticleManager particles;
    std::unique_ptr<Apple> apple;
    Flash flash;
    FloatingText floatingText;
    sf::View gameView;
    sf::Vector2f defaultCenter;
    DifficultyManager& difficulty;

    enum class Phase {
        PLAY,
        EXIT,
        PAUSE,
        GAMEOVER
    } phase;

    void restoreDefaults();
    void initVisuals(sf::RenderWindow& window);

    void handleEvents(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    void tick();

    void tickPlay();
    void tickStep();
    void handleSnakeCollision();
    void handleApple();
    float calculateSpeed(std::uint16_t score);

    void tickVisualUpdates();
    void tickGameOver();
public:
    Game(Configuration& config, sf::Font& font, DifficultyManager& difficulty);
    void start(sf::RenderWindow& window);
};