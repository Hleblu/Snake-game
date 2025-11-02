#pragma once
#include "apple.hpp"
#include "obstacle.hpp"
#include "renderResources.hpp"
#include "snake.hpp"
#include "soundManager.hpp"
#include "particleManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Game 
{
    sf::Clock clock;
    sf::Sprite background;
    sf::RectangleShape flashRect;
    SoundManager sManager;
    RenderResources renderResources;
    Snake snake;
    Obstacle obstacle;
    ParticleManager particles;
    std::unique_ptr<Apple> apple;

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