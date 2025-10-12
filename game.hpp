#pragma once
#include "apple.hpp"
#include "obstacle.hpp"
#include "snake.hpp"
#include "soundManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

class Game 
{
    sf::Clock clock;
    Snake snake;
    Obstacle obstacle;
    std::unique_ptr<Apple> apple;
    SoundManager sManager;
    sf::Sprite background;

    enum class State {
        PLAY,
        EXIT,
        PAUSE,
        GAMEOVER
    } state;

    void restoreDefaults();
    float calculateSpeed(const size_t& snakeDefaultSize);
    void initVisuals();
public:
    void start(sf::RenderWindow& window);
    Game();
};