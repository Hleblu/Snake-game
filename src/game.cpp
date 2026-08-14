#include "collisionManager.hpp"
#include "configuration.hpp"
#include "game.hpp"
#include "randomGenerator.hpp"
#include "renderResources.hpp"
#include "sounds/sound_eat.hpp"
#include "sounds/sound_gameover.hpp"
#include "sounds/sound_move.hpp"
#include "viewUtils.hpp"
#include <cmath>
#include <map>

namespace {
    constexpr size_t soundPoolSize = 5;
    constexpr size_t particlePoolSize = 120;
    constexpr size_t particlesPerEmission = 20;

    constexpr const char* SOUND_MOVE = "move";
    constexpr const char* SOUND_EAT = "eat";
    constexpr const char* SOUND_GAMEOVER = "gameover";
}

enum class Action
{
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Pause,
    Quit,
};

const std::map<sf::Keyboard::Key, Action> keyBinds
{
    { sf::Keyboard::Key::Left, Action::MoveLeft },
    { sf::Keyboard::Key::A, Action::MoveLeft },
    { sf::Keyboard::Key::Right, Action::MoveRight },
    { sf::Keyboard::Key::D, Action::MoveRight },
    { sf::Keyboard::Key::Up, Action::MoveUp },
    { sf::Keyboard::Key::W, Action::MoveUp },
    { sf::Keyboard::Key::Down, Action::MoveDown },
    { sf::Keyboard::Key::S, Action::MoveDown },
    { sf::Keyboard::Key::Escape, Action::Quit },
    { sf::Keyboard::Key::Space, Action::Pause }
};

void GameContext::reset(float startDelay)
{
    score = 0;
    shaderTime = 0.f;
    deltaTime = 0.f;
    updateAccumulator = 0.f;
    gameOverTimer = 0.f;
    updateDelay = startDelay;
}

Game::Game(Configuration& config, sf::Font& font, DifficultyManager& difficulty) :
    config(config),
    collision(),
    sounds(soundPoolSize),
    background(resources.createCheckerboardTexture()),
    phase(Phase::PLAY),
    particles(particlePoolSize),
    snake(config, collision, &resources.snakeShader),
    obstacle(config, collision, &resources.fadeShader),
    flash(config, sf::Color::White),
    floatingText(config, &resources.floatingTextFadeShader, font),
    difficulty(difficulty)
{
    resources.loadSnakeShader();
    resources.loadAppleTexture();
    resources.loadFadeShader();
    resources.loadSpriteFadeShader();
    resources.loadFloatingTextFadeShader();

    sounds.addSound(SOUND_MOVE, sound_move, sound_move_len);
    sounds.addSound(SOUND_EAT, sound_eat, sound_eat_len);
    sounds.addSound(SOUND_GAMEOVER, sound_gameover, sound_gameover_len);
}

void Game::start(sf::RenderWindow& window)
{
    difficulty.onStart();
    restoreDefaults();
    initVisuals(window);

    clock.restart();
    while (window.isOpen() && phase != Phase::EXIT)
    {
        handleEvents(window);

        tick();

        render(window);
    }
    if (!snake.isWaitingForFirstMove())
        difficulty.onEnd();
}

void Game::restoreDefaults() {
    context.reset(config.getStartDelay());
    collision.init(config.getRows(), config.getColumns());
    particles.clearParticles();
    snake.restoreDefaultValues();
    obstacle.restoreDefaultValues();
    floatingText.initAppearence();
    floatingText.hide();
    apple = AppleFactory::createRandomApple(
        config,
        collision,
        resources.appleTexture,
        &resources.spriteFadeShader,
        0
    );
    difficulty.updateExpected(
        snake.getHead(),
        apple->getPosition(),
        collision.getOccupancyRate(),
        context.updateDelay
    );
    phase = Phase::PLAY;
}

void Game::initVisuals(sf::RenderWindow& window)
{
    background.updateDimensions(
        config.getCellSize(),
        sf::IntRect({0, 0}, {config.getFieldDimensions<int>()})
    );

    resources.updateCheckerboardTexture(
        background.getTexture(),
        config.getCurrentTheme().mainColor,
        config.getCurrentTheme().secondColor
    );
    
    ViewUtils::normalizeView(gameView, config.getFieldDimensions<float>(), sf::Vector2f(window.getSize()));
    defaultCenter = gameView.getCenter();
    window.setView(gameView);
}

void Game::handleEvents(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) window.close();

        if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            ViewUtils::normalizeView(
                gameView,
                config.getFieldDimensions<float>(),
                sf::Vector2f(resized->size)
            );
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            auto it = keyBinds.find(keyPressed->code);
            if (it == keyBinds.end()) continue;

            if (phase == Phase::PLAY) {
                switch (it->second)
                {
                case Action::MoveLeft:
                    if (!snake.isWaitingForFirstMove()) // prevent gameover, since snake starts facing righwards
                        snake.setNextDirection(Direction::Val::LEFT);
                    break;
                case Action::MoveRight:
                    snake.setNextDirection(Direction::Val::RIGHT);
                    break;
                case Action::MoveUp:
                    snake.setNextDirection(Direction::Val::UP);
                    break;
                case Action::MoveDown:
                    snake.setNextDirection(Direction::Val::DOWN);
                    break;
                }
            }

            switch (it->second)
            {
            case Action::Quit:
                phase = Phase::EXIT;
                break;
            case Action::Pause:
                if (phase != Phase::GAMEOVER)
                    phase = (phase == Phase::PLAY) ? Phase::PAUSE : Phase::PLAY;
                break;
            }
        }
    }
}

void Game::render(sf::RenderWindow& window)
{
    window.setView(gameView);

    window.clear(config.getCurrentTheme().obstacleColor);

    window.draw(background);
    window.draw(particles);
    window.draw(*apple);
    window.draw(obstacle);
    window.draw(snake);
    window.draw(floatingText);

    if (phase == Phase::GAMEOVER)
        window.draw(flash);

    window.display();
}

void Game::tick()
{
    context.deltaTime = clock.restart().asSeconds();
    if (phase != Phase::PAUSE)
        context.shaderTime += context.deltaTime;

    if (phase == Phase::PLAY)
        tickPlay();

    if (phase != Phase::PAUSE)
        tickVisualUpdates();

    if (phase == Phase::GAMEOVER)
        tickGameOver();
}

void Game::tickPlay()
{
    if (!snake.isWaitingForFirstMove())
        difficulty.updateCurrent(context.deltaTime);

    context.updateAccumulator += context.deltaTime;

    if (snake.updateDirection())
        sounds.playSound(SOUND_MOVE);

    while (context.updateAccumulator >= context.updateDelay)
        tickStep();
}

void Game::tickStep()
{
    context.updateAccumulator -= context.updateDelay;

    snake.move();

    if (snake.hasCollided()) {
        handleSnakeCollision();
        return;
    }

    if (apple->isEaten())
        handleApple();
}

void Game::handleSnakeCollision()
{
    snake.triggerDeath(context.shaderTime);
    phase = Phase::GAMEOVER;
    sounds.playSound(SOUND_GAMEOVER);
}

void Game::handleApple()
{
    context.score++;
    context.updateDelay = calculateSpeed(context.score);

    floatingText.updateLabel(
        std::to_string(context.score),
        snake.getHeadCenter()
    );

    particles.emit(
        particlesPerEmission,
        snake.getHeadCenter(),
        config.getParticleSize(),
        config.getCurrentTheme().appleColor,
        config.getParticleSpeedMin(),
        config.getParticleSpeedMax()
    );

    sounds.playSound(SOUND_EAT);
    apple->applyEffect(snake);
    apple = AppleFactory::createRandomApple(
        config,
        collision,
        resources.appleTexture,
        &resources.spriteFadeShader,
        context.shaderTime
    );

    if (config.areObstaclesEnabled() && (context.score & 1) == 1)
        obstacle.generateNewPosition(context.shaderTime);

    difficulty.updateExpected(
        snake.getHead(),
        apple->getPosition(),
        collision.getOccupancyRate(),
        context.updateDelay
    );
}

float Game::calculateSpeed(std::uint16_t score)
{
    const float sizeBonus = std::pow(config.delayDecreaseStep, score);
    return config.getStartDelay() / difficulty.getModifier() * sizeBonus * apple->getSpeedBonus();
}

void Game::tickVisualUpdates()
{
    apple->updateShader(context.shaderTime);
    obstacle.updateShader(context.shaderTime);
    snake.updateVertices(context.updateAccumulator / context.updateDelay);
    particles.update(context.deltaTime);
    floatingText.updateShader(context.deltaTime);

    if (phase == Phase::GAMEOVER) {
        ViewUtils::shakeView(
            gameView,
            defaultCenter,
            config.getShakeIntensity(),
            snake.getDirection(),
            context.gameOverTimer,
            config.getShakeDuration()
        );

        snake.updateShader(context.shaderTime);
        flash.updateAnim(context.gameOverTimer);
    }
}

void Game::tickGameOver()
{
    context.gameOverTimer += context.deltaTime;
    if (context.gameOverTimer >= config.gameOverDelay)
        phase = Phase::EXIT;
}
