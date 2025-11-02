#include "collisionManager.hpp"
#include "configuration.hpp"
#include "game.hpp"
#include "renderResources.hpp"
#include "randomGenerator.hpp"
#include "Resources/Sounds/sound_eat.hpp"
#include "Resources/Sounds/sound_gameover.hpp"
#include "Resources/Sounds/sound_move.hpp"

constexpr size_t soundBufferSize = 5;
constexpr float gameOverDelay = 0.5f; // in seconds

void Game::start(sf::RenderWindow& window)
{
    restoreDefaults();
    initVisuals();

    float deltaTime = 0;
    float gameUpdateAccumulator = 0;
    float gameOverTimer = 0;
    float totalTime = 0;
    float currentDelay = config.getStartDelay();
    float appleSpawnTime = 0;

    const size_t snakeDefaultSize = snake.getSize();

    clock.restart();
    while (window.isOpen() && state != State::EXIT)
    {
        while (const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>()) window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                switch (keyPressed->code)
                {
                    case sf::Keyboard::Key::Left:
                        if (!snake.firstTimeMoving()) // prevent gameover, since snake starts facing righwards
                            snake.setNextDirection(Snake::Direction::LEFT);
                        break;
                    case sf::Keyboard::Key::Right:
                        snake.setNextDirection(Snake::Direction::RIGHT);
                        break;
                    case sf::Keyboard::Key::Up:
                        snake.setNextDirection(Snake::Direction::UP);
                        break;
                    case sf::Keyboard::Key::Down:
                        snake.setNextDirection(Snake::Direction::DOWN);
                        break;
                    case sf::Keyboard::Key::Escape:
                        state = State::EXIT;
                        break;
                    case sf::Keyboard::Key::Space:
                        if (state != State::GAMEOVER)
                            state = (state == State::PLAY) ? State::PAUSE : State::PLAY;
                        break;
                }
        }

        deltaTime = clock.restart().asSeconds();
        totalTime += deltaTime;
        if (state == State::PLAY) {
            gameUpdateAccumulator += deltaTime;

            if (snake.updateDirection()) {
                sManager.playSound("move");
            }

            if (gameUpdateAccumulator >= currentDelay) {

                gameUpdateAccumulator -= currentDelay;

                snake.move();

                if (snake.hasCollided()) {
                    state = State::GAMEOVER;
                    sManager.playSound("gameover");
                }

                if (apple->isEaten()) {
                    currentDelay = calculateSpeed(snakeDefaultSize);

                    const float minSpeed = config.getCellSize() * 1.f;
                    const float maxSpeed = config.getCellSize() * 3.f;
                    particles.emit(1500, snake.getPositionInfront(), config.getAppleColor(), minSpeed, maxSpeed);

                    sManager.playSound("eat");
                    apple->applyEffect(snake);
                    apple = AppleFactory::createRandomApple(renderResources.appleTexture);
                    appleSpawnTime = totalTime;

                    if (config.areObstaclesEnabled() && (snake.getSize() & 1) == 0)
                        obstacle.generateNewPosition(totalTime);
                }
            }
        }
;
        const float appleAnimProgress = (totalTime - appleSpawnTime) / currentDelay;
        renderResources.spriteFadeShader.setUniform("animProgress", appleAnimProgress);
        renderResources.fadeShader.setUniform("currentTime", totalTime);
        snake.updateVertices(gameUpdateAccumulator / currentDelay);
        particles.update(deltaTime);

        window.clear(config.getSecondColor());

        window.draw(background);
        window.draw(particles);
        window.draw(*apple, &renderResources.spriteFadeShader);
        window.draw(obstacle, &renderResources.fadeShader);
        window.draw(snake, &renderResources.snakeShader);

        if (state == State::GAMEOVER) {
            const float alpha = (1.f - (gameOverTimer / gameOverDelay)) * 115.f; // 115.f out of 255.f maximum alpha
            sf::Color flashColor = sf::Color(255, 255, 255, alpha);
            flashRect.setFillColor(flashColor);
            window.draw(flashRect);

            gameOverTimer += deltaTime;
            if (gameOverTimer >= gameOverDelay) state = State::EXIT;
        }

        window.display();
    }
}

void Game::restoreDefaults() {
    collisionManager.clearMap();
    snake.restoreDefaultValues();
    obstacle.restoreDefaultValues();
    apple = AppleFactory::createRandomApple(renderResources.appleTexture);
    state = State::PLAY;
}

float Game::calculateSpeed(const size_t& snakeDefaultSize)
{
    const size_t applesEaten = snake.getSize() - snakeDefaultSize;
    const float sizeBonus = std::pow(config.delayDecreaseStep, applesEaten);
    return config.getStartDelay() * sizeBonus * apple->getSpeedBonus();
}

void Game::initVisuals()
{
    background.setScale(sf::Vector2f(config.getCellSize(), config.getCellSize()));
    background.setTextureRect({ { 0, 0 }, { config.width, config.height } });
    background.setColor(config.getMainColor());

    renderResources.snakeShader.setUniform("startColor", sf::Glsl::Vec4(config.getSnakeColor()));
    renderResources.snakeShader.setUniform("endColor", sf::Glsl::Vec4(config.getSnakeColorEnd()));

    renderResources.fadeShader.setUniform("duration", config.getStartDelay());

    flashRect.setSize({ config.width * 1.f, config.height * 1.f });
}

Game::Game()
    : sManager(soundBufferSize),
    background(renderResources.backgroundTexture),
    state(State::PLAY),
    particles(4500)
{
	renderResources.loadSnakeShader();
	renderResources.createBackgroundTexture();
    renderResources.loadAppleTexture();
    renderResources.loadFadeShader();
    renderResources.loadSpriteFadeShader();

    sManager.addSound("move", sound_move, sound_move_len);
    sManager.addSound("eat", sound_eat, sound_eat_len);
    sManager.addSound("gameover", sound_gameover, sound_gameover_len);
}