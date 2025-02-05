#include "game.h"

void Game::showBackground()
{
    backgroundVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    backgroundVertices.resize(vertexGrid);
    for (int x = 0; x < gridX; ++x) {
        for (int y = 0; y < gridY; ++y) {
            if ((x + y) % 2 == 0) { 
                float posX = x * size;
                float posY = y * size;

                sf::Vertex* triangles = &backgroundVertices[(x + y * gridX) * 6];
                triangles[0].position = sf::Vector2f(posX, posY);
                triangles[1].position = sf::Vector2f(posX + size, posY);
                triangles[2].position = sf::Vector2f(posX + size, posY + size);
                triangles[3].position = sf::Vector2f(posX + size, posY + size);
                triangles[4].position = sf::Vector2f(posX, posY + size);
                triangles[5].position = sf::Vector2f(posX, posY);

                triangles[0].color = sf::Color(172, 206, 94);
                triangles[1].color = sf::Color(172, 206, 94);
                triangles[2].color = sf::Color(172, 206, 94);
                triangles[3].color = sf::Color(172, 206, 94);
                triangles[4].color = sf::Color(172, 206, 94);
                triangles[5].color = sf::Color(172, 206, 94);
            }
        }
    }
}

void Game::playSound(sf::SoundBuffer& buffer) {
    sf::Sound someSound(buffer);
    soundsArray.push_front(someSound);
    soundsArray.front().play();
    while (!soundsArray.empty() && soundsArray.back().getStatus() != sf::Sound::Status::Playing) soundsArray.pop_back();
}

void Game::Start(sf::RenderWindow& window)
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

        if (snake.nextDirection != snake.direction && snake.nextDirection % 2 != snake.previousDirection % 2) {
            snake.direction = snake.nextDirection;
            playSound(moveSoundBuffer);
        }

        if (timer >= delay) {
            timer = 0;
            snake.move();
            if (snake.checkCollision()) {
                playSound(gameOverSoundBuffer);
                snake.setDefaults();
                apple.genCords();
                sf::sleep(sf::seconds(0.5));
                soundsArray.clear();
                return;
            }
            snake.updateVertexArray();
            if (snake.segments[0].x == apple.x
                && snake.segments[0].y == apple.y) {
                apple.genCords();
                playSound(eatSoundBuffer);
                snake.grow();
            }
        }
        window.clear(sf::Color(114, 183, 106));
        window.draw(backgroundVertices);
        window.draw(apple);
        window.draw(snake);
        window.display();
    }
}

Game::Game() : apple(snake) {  
    showBackground();
    eatSoundBuffer.loadFromFile("Resources/Sounds/sound_food.ogg");
    gameOverSoundBuffer.loadFromFile("Resources/Sounds/sound_gameover.ogg");
    moveSoundBuffer.loadFromFile("Resources/Sounds/sound_move.ogg");
}