#include "game.h"

void Game::playSound(sf::SoundBuffer& buffer) {
    for (auto& sound : soundsArray) {
        if (sound.getStatus() == sf::Sound::Status::Stopped) {
            sound.setBuffer(buffer);
            sound.play();
            return;
        }
    }

	auto oldestSound = std::min_element(soundsArray.begin(), soundsArray.end(), [](const sf::Sound& a, const sf::Sound& b) {
		return a.getPlayingOffset() > b.getPlayingOffset();
	});

	oldestSound->setBuffer(buffer);
    oldestSound->play();
}

void Game::restoreDefaults() {
    snake.restoreDefaultValues();
    apple.generateNewPosition();
    sf::sleep(sf::seconds(0.5f));
    isGameOver = false;
}

void Game::start(sf::RenderWindow& window)
{
    float deltaTime = 0, gameUpdateAccumulator = 0, animationAccumulator = 0, currentDelay = config->delay;
	const float animationFrameTime = 1.0f / config->animationFrameTime;

    static sf::Sprite background(renderer->backgroundTexture);
    background.setScale({ static_cast<float>(config->size), static_cast<float>(config->size)});
    background.setTextureRect({ { 0, 0 }, { static_cast<int>(config->width), static_cast<int>(config->height) } });
	background.setColor(config->mainColor);

    renderer->gradientShader.setUniform("startColor", sf::Glsl::Vec4(config->snakeColor));
	renderer->gradientShader.setUniform("endColor", sf::Glsl::Vec4(config->snakeColorEnd));

    while (window.isOpen() && !isGameOver)
    {
        deltaTime = clock.restart().asSeconds();
        gameUpdateAccumulator += deltaTime;
        animationAccumulator += deltaTime;

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

        if (gameUpdateAccumulator >= currentDelay) {
            gameUpdateAccumulator -= currentDelay;
            snake.move();
            if (snake.hasCollided()) {
                isGameOver = true;
                playSound(gameOverSoundBuffer);
            }
            snake.updateVertices();
            if (apple.isEaten()) {
                currentDelay *= config->speedDecreaseStep;
                apple.generateNewPosition();
                playSound(eatSoundBuffer);
                snake.grow();
            }
        }
        else if (animationAccumulator >= animationFrameTime) {
            snake.updateVertices(gameUpdateAccumulator / currentDelay); 
            animationAccumulator -= animationFrameTime;
        }

        window.clear(config->secondColor);
        window.draw(background);
        window.draw(apple);
        window.draw(snake, &renderer->gradientShader);
        window.display();
    }
    restoreDefaults();
}

Game::Game() : apple(snake) {
	renderer->loadGradientShader();
	renderer->createBackgroundTexture();

    if (!eatSoundBuffer.loadFromMemory(sound_food_ogg, sound_food_ogg_len)) throw std::runtime_error("couldn\'t load sound eat");
    if (!gameOverSoundBuffer.loadFromMemory(sound_gameover_ogg, sound_gameover_ogg_len)) throw std::runtime_error("couldn\'t load sound gameOver");
    if (!moveSoundBuffer.loadFromMemory(sound_move_ogg, sound_move_ogg_len)) throw std::runtime_error("couldn\'t load sound move");
    soundsArray.resize(5, sf::Sound(moveSoundBuffer));
}