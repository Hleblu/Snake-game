#include "game.h"

void Game::initializeBackground()
{
    sf::Shader checkboardShader;
    if (!checkboardShader.loadFromMemory(R"(
        uniform vec2 gridSize;
        uniform vec2 windowSize;

        void main() {
            vec2 cell = floor(gl_FragCoord.xy * gridSize / windowSize);
            float checker = mod(cell.x + cell.y, 2.0);

            vec3 finalColor = mix(vec3(0.675, 0.808, 0.369), vec3(0.447, 0.718, 0.416), checker);

            gl_FragColor = vec4(finalColor, 1.0);
        }   
)", sf::Shader::Type::Fragment)) return;
    checkboardShader.setUniform("gridSize", sf::Vector2f(gridX, gridY));
    checkboardShader.setUniform("windowSize", sf::Vector2f(width/2, height/2));
    sf::RenderTexture texture({ width/2, height/2 });
    sf::RectangleShape someRectangle({ width/2, height/2 });
    texture.clear();
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
    static sf::Sprite background(backgroundTexture);
    background.setScale({ 2,2 });
    while (window.isOpen() && !isGameOver)
    {
        time = clock.restart().asSeconds(); // as described in the SFML article, returns time like clock.getElapsedTime().asSeconds(), ne tupi potim, Hlib;
        timer += time;
        animationTimer += time;

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
            clearSoundsArray();
            snake.move();
            if (snake.hasCollided()) {
                isGameOver = true;
                playSound(gameOverSoundBuffer);
            }
            snake.updateVertexArray();
            if (apple.isEaten()) {
                apple.generateNewPosition();
                playSound(eatSoundBuffer);
                snake.grow();
            }
        }
        else if (animationTimer >= 0.0416) /* 24 fps */ {
            snake.updateVertexArray(timer / delay); animationTimer = 0;
        }

        window.clear();
        window.draw(background);
        window.draw(apple);
        window.draw(snake);
        window.display();
    }
    restoreDefaults();
}

Game::Game() : apple(snake), someSound(moveSoundBuffer) {  
    initializeBackground();
    if (!eatSoundBuffer.loadFromMemory(sound_food_ogg, sound_food_ogg_len)) return;
    if (!gameOverSoundBuffer.loadFromMemory(sound_gameover_ogg, sound_gameover_ogg_len)) return;
    if (!moveSoundBuffer.loadFromMemory(sound_move_ogg, sound_move_ogg_len)) return;
}