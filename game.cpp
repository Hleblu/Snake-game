#include "game.h"

void Game::initializeBackground()
{
    sf::Shader checkboardShader;
    if (!checkboardShader.loadFromMemory(R"(
        uniform vec2 gridSize;
        uniform vec2 windowSize;
        uniform vec3 fColor;
        uniform vec3 sColor;

        void main() {
            vec2 cell = floor(gl_FragCoord.xy * gridSize / windowSize);
            float checker = mod(cell.x + cell.y, 2.0);

            vec3 finalColor = mix(fColor, sColor, checker);

            gl_FragColor = vec4(finalColor, 1.0);
        }   
)", sf::Shader::Type::Fragment)) return;
    checkboardShader.setUniform("gridSize", sf::Vector2f(gridX, gridY));
    checkboardShader.setUniform("windowSize", sf::Vector2f(width / 2, height / 2));
    checkboardShader.setUniform("fColor", sf::Vector3f(mainColor.r / 255.0f, mainColor.g / 255.0f, mainColor.b / 255.0f));
    checkboardShader.setUniform("sColor", sf::Vector3f(secondColor.r / 255.0f, secondColor.g / 255.0f, secondColor.b / 255.0f));

    sf::RenderTexture texture({ width/2, height/2 });
    sf::RectangleShape someRectangle({ width/2, height/2 });

    texture.clear();
    texture.draw(someRectangle, &checkboardShader);
    texture.display();

    backgroundTexture = texture.getTexture();
}

void Game::changeDelay(float d)
{
    delay = d;
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
    mainColor = { 114, 183, 106 };
    secondColor = { 172, 206, 94 };

    initializeBackground();

    if (!eatSoundBuffer.loadFromMemory(sound_food_ogg, sound_food_ogg_len)) return;
    if (!gameOverSoundBuffer.loadFromMemory(sound_gameover_ogg, sound_gameover_ogg_len)) return;
    if (!moveSoundBuffer.loadFromMemory(sound_move_ogg, sound_move_ogg_len)) return;
}