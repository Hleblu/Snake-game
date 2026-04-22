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

constexpr size_t soundPoolSize = 5;
constexpr size_t particlePoolSize = 120;
constexpr size_t particlesPerEmission = 20;
constexpr size_t shakeIntensity = 3;

constexpr const char* SOUND_MOVE = "move";
constexpr const char* SOUND_EAT = "eat";
constexpr const char* SOUND_GAMEOVER = "gameover";

void Game::start(sf::RenderWindow& window)
{
    difficulty->onStart();
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
        difficulty->onEnd();
}

void Game::restoreDefaults() {
    context.reset(config->getStartDelay());
    collision->init(config->getRows(), config->getColumns());
    particles.clearParticles();
    snake.restoreDefaultValues();
    obstacle.restoreDefaultValues();
    floatingText.hide();
    apple = AppleFactory::createRandomApple(
        config,
        collision.get(),
        resources.appleTexture,
        &resources.spriteFadeShader,
        0
    );
    difficulty->updateExpected(
        snake.getHead(),
        apple->getPosition(),
        collision->getOccupancyRate(),
        context.updateDelay
    );
    phase = Phase::PLAY;
}

float Game::calculateSpeed(std::uint16_t score)
{
    const float sizeBonus = std::pow(config->delayDecreaseStep, score);
    return config->getStartDelay() / difficulty->getModificator() * sizeBonus * apple->getSpeedBonus();
}

void Game::initVisuals(sf::RenderWindow& window)
{
    background.setScale(sf::Vector2f(config->getCellSize(), config->getCellSize()));
    background.setTextureRect({ { 0, 0 }, { config->width, config->height } });
    background.setColor(config->getCurrentTheme().mainColor);
    
    ViewUtils::normalizeView(gameView, 
        { 
            static_cast<float>(config->width),
            static_cast<float>(config->height)
        }
    );
    defaultCenter = gameView.getCenter();
    window.setView(gameView);
}

void Game::handleEvents(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) window.close();
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (phase == Phase::PLAY) {
                switch (keyPressed->code)
                {
                case sf::Keyboard::Key::Left:
                    if (!snake.isWaitingForFirstMove()) // prevent gameover, since snake starts facing righwards
                        snake.setNextDirection(Direction::Val::LEFT);
                    break;
                case sf::Keyboard::Key::Right:
                    snake.setNextDirection(Direction::Val::RIGHT);
                    break;
                case sf::Keyboard::Key::Up:
                    snake.setNextDirection(Direction::Val::UP);
                    break;
                case sf::Keyboard::Key::Down:
                    snake.setNextDirection(Direction::Val::DOWN);
                    break;
                }
            }

            switch (keyPressed->code)
            {
            case sf::Keyboard::Key::Escape:
                phase = Phase::EXIT;
                break;
            case sf::Keyboard::Key::Space:
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

    window.clear(config->getCurrentTheme().secondColor);

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
        difficulty->updateCurrent(context.deltaTime);

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
        config->getParticleSize(),
        config->getCurrentTheme().appleColor,
        config->getParticleSpeedMin(),
        config->getParticleSpeedMax()
    );

    sounds.playSound(SOUND_EAT);
    apple->applyEffect(snake);
    apple = AppleFactory::createRandomApple(
        config,
        collision.get(),
        resources.appleTexture,
        &resources.spriteFadeShader,
        context.shaderTime
    );

    if (config->areObstaclesEnabled() && (context.score & 1) == 1)
        obstacle.generateNewPosition(context.shaderTime);

    difficulty->updateExpected(
        snake.getHead(),
        apple->getPosition(),
        collision->getOccupancyRate(),
        context.updateDelay
    );
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
            shakeIntensity,
            snake.getDirection(),
            context.gameOverTimer,
            config->gameOverDelay
        );

        snake.updateShader(context.shaderTime);
        flash.updateAnim(context.gameOverTimer);
    }
}

void Game::tickGameOver()
{
    context.gameOverTimer += context.deltaTime;
    if (context.gameOverTimer >= config->gameOverDelay)
        phase = Phase::EXIT;
}

Game::Game(Configuration* config, sf::Font* font, DifficultyManager* difficulty)
    : sounds(soundPoolSize),
    background(resources.backgroundTexture),
    phase(Phase::PLAY),
    particles(particlePoolSize),
    config(config),
    collision(std::make_unique<CollisionManager>()),
    snake(config, collision.get(), &resources.snakeShader),
    obstacle(config, collision.get(), &resources.fadeShader),
    flash(config, sf::Color::White),
    floatingText(config, &resources.floatingTextFadeShader, font),
    difficulty(difficulty)
{
	resources.loadSnakeShader();
	resources.createBackgroundTexture();
    resources.loadAppleTexture();
    resources.loadFadeShader();
    resources.loadSpriteFadeShader();
    resources.loadFloatingTextFadeShader();

    sounds.addSound(SOUND_MOVE, sound_move, sound_move_len);
    sounds.addSound(SOUND_EAT, sound_eat, sound_eat_len);
    sounds.addSound(SOUND_GAMEOVER, sound_gameover, sound_gameover_len);
}

void GameContext::reset(float startDelay)
{
    score = 0;
    shaderTime = 0.f;
    deltaTime = 0.f;
    updateAccumulator = 0.f;
    gameOverTimer = 0.f;
    updateDelay = startDelay;
}
