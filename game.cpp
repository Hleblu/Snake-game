#include "game.h"

void Game::playSound(sf::SoundBuffer& buffer) {
    someSound.setBuffer(buffer);
    soundsArray.emplace_front(std::move(someSound));
    soundsArray.front().play();
}

void Game::clearSoundsArray() {
    while (!soundsArray.empty() && soundsArray.back().getStatus() != sf::Sound::Status::Playing) soundsArray.pop_back();
}

void Game::restoreDefaults() {
    snake.restoreDefaultValues();
    apple.generateNewPosition();
    sf::sleep(sf::seconds(0.5f));
    soundsArray.clear();
    isGameOver = false;
}

void Game::start(sf::RenderWindow& window)
{
    float time = 0, timer = 0, animationTimer = 0, currentDelay = config->delay;
    static sf::Sprite background(renderer->backgroundTexture);
	background.setColor(config->mainColor);
    renderer->gradientShader.setUniform("color", sf::Glsl::Vec4(config->snakeColor));

    while (window.isOpen() && !isGameOver)
    {
        time = clock.restart().asSeconds();
        timer += time;
        animationTimer += time;

        while (const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>()) window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                switch (keyPressed->code)
                {
                    case sf::Keyboard::Key::Left:
                        if (!snake.firstMove) snake.nextDirection = Snake::LEFT; break;
                    case sf::Keyboard::Key::Right: snake.nextDirection = Snake::RIGHT; break;
                    case sf::Keyboard::Key::Up: snake.nextDirection = Snake::UP; break;
                    case sf::Keyboard::Key::Down: snake.nextDirection = Snake::DOWN; break;
                }
        }

        if (snake.canUpdateDirection()) {
            snake.direction = snake.nextDirection;
            playSound(moveSoundBuffer);
        }

        if (timer >= currentDelay) {
            timer = 0;
            clearSoundsArray();
            snake.move();
            if (snake.hasCollided()) {
                isGameOver = true;
                playSound(gameOverSoundBuffer);
            }
            snake.updateVertexArray();
            if (apple.isEaten()) {
                currentDelay *= 0.995f;
                apple.generateNewPosition();
                playSound(eatSoundBuffer);
                snake.grow();
            }
        }
        else if (animationTimer >= 0.0322f) /* 31 fps */ {
            snake.updateVertexArray(timer / currentDelay); 
            animationTimer = 0;
        }

        window.clear(config->secondColor);
        window.draw(background);
        window.draw(apple);
        window.draw(snake, &renderer->gradientShader);
        window.display();
    }
    restoreDefaults();
}

Game::Game() : apple(snake), someSound(moveSoundBuffer) {
	renderer->loadGradientShader();
	renderer->createBackgroundTexture();

    if (!eatSoundBuffer.loadFromMemory(sound_food_ogg, sound_food_ogg_len)) return;
    if (!gameOverSoundBuffer.loadFromMemory(sound_gameover_ogg, sound_gameover_ogg_len)) return;
    if (!moveSoundBuffer.loadFromMemory(sound_move_ogg, sound_move_ogg_len)) return;
}