#include "game.h"

void Game::initializeBackground()
{
	background.setSize({ width, height });
    if (!checkboardShader.loadFromFile("checkered.frag", sf::Shader::Type::Fragment)) return;
    checkboardShader.setUniform("gridSize", sf::Vector2f(gridX, gridY));
    checkboardShader.setUniform("windowSize", sf::Vector2f(width, height));
}

void Game::playSound(sf::SoundBuffer& buffer) {
    someSound.setBuffer(buffer);
    soundsArray.emplace_front(std::move(someSound));
    soundsArray.front().play();
    while (!soundsArray.empty() && soundsArray.back().getStatus() != sf::Sound::Status::Playing) soundsArray.pop_back();
}

void Game::start(sf::RenderWindow& window)
{
    while (window.isOpen())
    {
        time = clock.restart().asSeconds(); // as described in the SFML article, returns time like clock.getElapsedTime().asSeconds(), ne tupi potim, Hlib;
        timer += time;

        while (const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>()) window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                switch (keyPressed->code)
                {
                    case sf::Keyboard::Key::Left:
                        if (snake.previousDirection != Snake::NONE) snake.nextDirection = Snake::LEFT; break;
                    case sf::Keyboard::Key::Right: snake.nextDirection = Snake::RIGHT; break;
                    case sf::Keyboard::Key::Up: snake.nextDirection = Snake::UP; break;
                    case sf::Keyboard::Key::Down: snake.nextDirection = Snake::DOWN; break;
                }
        }

        if (snake.canUpdateDirection()) {
            snake.direction = snake.nextDirection;
            playSound(moveSoundBuffer);
        }

        if (timer >= delay) {
            timer = 0;
            snake.move();
            if (snake.hasCollided()) {
                playSound(gameOverSoundBuffer);
                snake.restoreDefaultValues();
                apple.generateNewPosition();
                sf::sleep(sf::seconds(0.5));
                soundsArray.clear();
                return;
            }
            snake.updateVertexArray();
            if (apple.isEaten()) {
                apple.generateNewPosition();
                playSound(eatSoundBuffer);
                snake.grow();
            }
        }
        window.clear();
        window.draw(background, &checkboardShader);
        window.draw(apple);
        window.draw(snake);
        window.display();
    }
}

Game::Game() : apple(snake), someSound(moveSoundBuffer) {  
    initializeBackground();
    if (!eatSoundBuffer.loadFromFile("Resources/Sounds/sound_food.ogg")) return;
    if (!gameOverSoundBuffer.loadFromFile("Resources/Sounds/sound_gameover.ogg")) return;
    if (!moveSoundBuffer.loadFromFile("Resources/Sounds/sound_move.ogg")) return;
}