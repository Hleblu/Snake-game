#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "apple.h"
#include "snake.h"
#include "obstacle.h"

class Apple;

class Game 
{
    sf::SoundBuffer eatSoundBuffer, gameOverSoundBuffer, moveSoundBuffer;
    std::vector<sf::Sound> soundsArray;
    void playSound(sf::SoundBuffer& buffer);

    sf::Clock clock;
    Snake snake;
    Obstacle obstacle;
    std::unique_ptr<Apple> apple;

    void restoreDefaults();
public:
    void start(sf::RenderWindow& window);
    Game();
};