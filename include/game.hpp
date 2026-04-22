#pragma once
#include "apple.hpp"
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
#include <SFML/Graphics/Sprite.hpp>
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
    Configuration* config;
    std::unique_ptr<CollisionManager> collision;
    GameContext context;
    sf::Clock clock;
    sf::Sprite background;
    SoundManager sounds;
    RenderResources resources;
    Snake snake;
    Obstacle obstacle;
    ParticleManager particles;
    std::unique_ptr<Apple> apple;
    Flash flash;
    FloatingText floatingText;
    sf::View gameView;
    sf::Vector2f defaultCenter;
    DifficultyManager* difficulty;

    enum class Phase {
        PLAY,
        EXIT,
        PAUSE,
        GAMEOVER
    } phase;

    void restoreDefaults();
    float calculateSpeed(std::uint16_t score);
    void initVisuals(sf::RenderWindow& window);

    void handleEvents(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    void tick();

    void tickPlay();
    void tickStep();
    void handleSnakeCollision();
    void handleApple();

    void tickVisualUpdates();
    void tickGameOver();
public:
    void start(sf::RenderWindow& window);
    Game(Configuration* config, sf::Font* font, DifficultyManager* difficulty);
};