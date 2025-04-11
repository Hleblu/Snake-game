#include "game.h"

void Game::initializeBackground()
{
    sf::Shader checkboardShader;
    if (!checkboardShader.loadFromMemory(R"(
        uniform float tileSize;
        void main() {
            vec2 cell = floor(gl_FragCoord.xy / tileSize);
            float alpha = 1.0 - mod(cell.x + cell.y, 2.0);
            gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
        }  
)", sf::Shader::Type::Fragment)) return;
    checkboardShader.setUniform("tileSize", static_cast<float>(config->size));

    sf::RenderTexture texture({ config->width, config->height });
    sf::RectangleShape someRectangle({ static_cast<float>(config->width), static_cast<float>(config->height) });

    texture.clear(sf::Color(0, 0, 0, 0));
    texture.draw(someRectangle, &checkboardShader);
    texture.display();

    backgroundTexture = texture.getTexture();
}

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
    sf::sleep(sf::seconds(0.5));
    soundsArray.clear();
    isGameOver = false;
}

void Game::start(sf::RenderWindow& window)
{
    float time = 0, timer = 0, animationTimer = 0, currentDelay = config->delay;
    static sf::Sprite background(backgroundTexture);
	background.setColor(config->mainColor);
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
                currentDelay *= 0.99f;
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
        window.draw(snake, &snake.colorShader);
        window.display();
    }
    restoreDefaults();
}

Game::Game(Configuration& config) : config(&config), snake(config), apple(snake), someSound(moveSoundBuffer) {
    initializeBackground();

    if (!eatSoundBuffer.loadFromMemory(sound_food_ogg, sound_food_ogg_len)) return;
    if (!gameOverSoundBuffer.loadFromMemory(sound_gameover_ogg, sound_gameover_ogg_len)) return;
    if (!moveSoundBuffer.loadFromMemory(sound_move_ogg, sound_move_ogg_len)) return;
}