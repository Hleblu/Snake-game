#include "collisionManager.hpp"
#include "configuration.hpp"
#include "game.hpp"
#include "randomGenerator.hpp"
#include "renderResources.hpp"
#include "Resources/Sounds/sound_eat.hpp"
#include "Resources/Sounds/sound_gameover.hpp"
#include "Resources/Sounds/sound_move.hpp"
#include "viewUtils.hpp"

constexpr size_t soundBufferSize = 5;

void Game::start(sf::RenderWindow& window)
{
    restoreDefaults();
    initVisuals(window);

    clock.restart();
    while (window.isOpen() && state != State::EXIT)
    {
        while (const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>()) window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (state == State::PLAY) {
                    switch (keyPressed->code)
                    {
                        case sf::Keyboard::Key::Left:
                            if (!snake.firstTimeMoving()) // prevent gameover, since snake starts facing righwards
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
                        state = State::EXIT;
                        break;
                    case sf::Keyboard::Key::Space:
                        if (state != State::GAMEOVER)
                            state = (state == State::PLAY) ? State::PAUSE : State::PLAY;
                        break;
                }
            }
        }

        context.deltaTime = clock.restart().asSeconds();
        if (state != State::PAUSE)
            context.totalTime += context.deltaTime;
        
        if (state == State::PLAY) {
            context.updateAccumulator += context.deltaTime;

            if (snake.updateDirection()) {
                sManager.playSound("move");
            }

            while (context.updateAccumulator >= context.delay) {

                context.updateAccumulator -= context.delay;

                snake.move();

                if (snake.hasCollided()) {
                    snake.triggerDeath(context.totalTime);
                    state = State::GAMEOVER;
                    sManager.playSound("gameover");
                }

                if (apple->isEaten()) {
                    context.score++;
                    context.delay = calculateSpeed(context.score);

                    floatingText.updateLabel(
                        std::to_string(context.score),
                        snake.getPositionInfront()
                    );

                    particles.emit(
                        20,
                        snake.getPositionInfront(),
                        config->getParticleSize(),
                        config->getCurrentTheme().appleColor,
                        config->getParticleSpeedMin(),
                        config->getParticleSpeedMax()
                    );

                    sManager.playSound("eat");
                    apple->applyEffect(snake);
                    apple = AppleFactory::createRandomApple(
                        config,
                        collision.get(),
                        resources.appleTexture,
                        &resources.spriteFadeShader,
                        context.totalTime
                    );

                    if (config->areObstaclesEnabled() && (context.score & 1) == 1)
                        obstacle.generateNewPosition(context.totalTime);
                }
            }
        }

        if (state != State::PAUSE) {
            apple->updateShader(context.totalTime);
            obstacle.updateShader(context.totalTime);
            snake.updateVertices(context.updateAccumulator / context.delay);
            particles.update(context.deltaTime);
            floatingText.updateShader(context.deltaTime);
        }

        window.clear(config->getCurrentTheme().secondColor);

        window.draw(background);
        window.draw(particles);
        window.draw(*apple);
        window.draw(obstacle);
        window.draw(snake);
        window.draw(floatingText);

        if (state == State::GAMEOVER) {
            ViewUtils::shakeView(
                gameView,
                defCenter,
                window,
                3,
                snake.getDirection(),
                context.gameOverTimer,
                config->gameOverDelay
            );

            snake.updateShader(context.totalTime);
            flash.updateAnim(context.gameOverTimer);
            window.draw(flash);

            context.gameOverTimer += context.deltaTime;
            if (context.gameOverTimer >= config->gameOverDelay) state = State::EXIT;
        }

        window.display();
    }
}

void Game::restoreDefaults() {
    context.reset(config->getStartDelay());
    collision->init(config->getRows(), config->getColumns());
    particles.clearParticles();
    snake.restoreDefaultValues();
    obstacle.restoreDefaultValues();
    apple = AppleFactory::createRandomApple(
        config,
        collision.get(),
        resources.appleTexture,
        &resources.spriteFadeShader,
        0
    );
    state = State::PLAY;
}

float Game::calculateSpeed(std::uint16_t score)
{
    const float sizeBonus = std::pow(config->delayDecreaseStep, score);
    return config->getStartDelay() * sizeBonus * apple->getSpeedBonus();
}

void Game::initVisuals(sf::RenderWindow& window)
{
    background.setScale(sf::Vector2f(config->getCellSize(), config->getCellSize()));
    background.setTextureRect({ { 0, 0 }, { config->width, config->height } });
    background.setColor(config->getCurrentTheme().mainColor);
    
    floatingText.initAppearence();

    ViewUtils::normalizeView(gameView, 
        { 
            static_cast<float>(config->width),
            static_cast<float>(config->height)
        }
    );
    defCenter = gameView.getCenter();
    window.setView(gameView);
}

Game::Game(Configuration* config, sf::Font* font)
    : sManager(soundBufferSize),
    background(resources.backgroundTexture),
    state(State::PLAY),
    particles(200),
    config(config),
    collision(std::make_unique<CollisionManager>()),
    snake(config, collision.get(), &resources.snakeShader),
    obstacle(config, collision.get(), &resources.fadeShader),
    flash(config, sf::Color::White),
    floatingText(config, &resources.floatingTextFadeShader, font)
{
	resources.loadSnakeShader();
	resources.createBackgroundTexture();
    resources.loadAppleTexture();
    resources.loadFadeShader();
    resources.loadSpriteFadeShader();
    resources.loadFloatingTextFadeShader();

    sManager.addSound("move", sound_move, sound_move_len);
    sManager.addSound("eat", sound_eat, sound_eat_len);
    sManager.addSound("gameover", sound_gameover, sound_gameover_len);
}

void GameContext::reset(float startDelay)
{
    score = 0;
    totalTime = 0;
    deltaTime = 0;
    updateAccumulator = 0;
    gameOverTimer = 0;
    delay = startDelay;
}
