#include "collisionManager.hpp"
#include "configuration.hpp"
#include "game.hpp"
#include "renderResources.hpp"
#include "Resources/Sounds/sound_food.c"
#include "Resources/Sounds/sound_gameover.c"
#include "Resources/Sounds/sound_move.c"
#include <SFML/System/Sleep.hpp>

constexpr size_t soundBufferSize = 5;
constexpr float gameOverDelay = 0.5f; // in seconds

void Game::start(sf::RenderWindow& window)
{
    restoreDefaults();
    initVisuals();

    float deltaTime = 0;
    float gameUpdateAccumulator = 0;
    float gameOverTimer = 0;
    float currentDelay = config.delay;

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
                        if (!snake.firstMove) // prevent gameover, since snake starts facaing righwards
                            snake.nextDirection = Snake::Direction::LEFT;
                        break;
                    case sf::Keyboard::Key::Right:
                        snake.nextDirection = Snake::Direction::RIGHT;
                        break;
                    case sf::Keyboard::Key::Up:
                        snake.nextDirection = Snake::Direction::UP;
                        break;
                    case sf::Keyboard::Key::Down:
                        snake.nextDirection = Snake::Direction::DOWN;
                        break;
                    case sf::Keyboard::Key::Escape:
                        state = State::EXIT;
                        break;
                    case sf::Keyboard::Key::Space:
                        state = (state == State::PLAY) ? State::PAUSE : State::PLAY;
                        break;
                }
        }

        deltaTime = clock.restart().asSeconds();
        if (state == State::PLAY) {
            gameUpdateAccumulator += deltaTime;

            if (snake.canUpdateDirection()) {
                snake.direction = snake.nextDirection;
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

                    sManager.playSound("eat");
                    apple->applyEffect(snake);
                    apple = AppleFactory::createRandomApple();

                    if (config.obstaclesEnabled && (snake.getSize() & 1) == 0)
                        obstacle.generateNewPosition();
                }
            }
        }

        snake.updateVertices(gameUpdateAccumulator / currentDelay);

        window.clear(config.currentTheme->secondColor);
        window.draw(background);
        window.draw(*apple);
        window.draw(obstacle);
        window.draw(snake, &renderResources.gradientShader);
        window.display();

        if (state == State::GAMEOVER) {
            gameOverTimer += deltaTime;
            if (gameOverTimer >= gameOverDelay) state = State::EXIT;
        }
    }
}

void Game::restoreDefaults() {
    collisionManager.clearMap();
    snake.restoreDefaultValues();
    obstacle.restoreDefaultValues();
    apple = AppleFactory::createRandomApple();
    state = State::PLAY;
}

float Game::calculateSpeed(const size_t& snakeDefaultSize)
{
    const size_t applesEaten = snake.getSize() - snakeDefaultSize;
    const float sizeBonus = std::pow(config.delayDecreaseStep, applesEaten);
    return config.delay * sizeBonus * apple->getSpeedBonus();
}

void Game::initVisuals()
{
    background.setScale(sf::Vector2f(config.size, config.size));
    background.setTextureRect({ { 0, 0 }, { config.width, config.height } });
    background.setColor(config.currentTheme->mainColor);

    renderResources.gradientShader.setUniform("startColor", sf::Glsl::Vec4(config.currentTheme->snakeColor));
    renderResources.gradientShader.setUniform("endColor", sf::Glsl::Vec4(config.currentTheme->snakeColorEnd));
}

Game::Game() : sManager(soundBufferSize), background(renderResources.backgroundTexture), state(State::PLAY) {
	renderResources.loadGradientShader();
	renderResources.createBackgroundTexture();

    sManager.addSound("move", sound_move_ogg, sound_move_ogg_len);
    sManager.addSound("eat", sound_food_ogg, sound_food_ogg_len);
    sManager.addSound("gameover", sound_gameover_ogg, sound_gameover_ogg_len);
}