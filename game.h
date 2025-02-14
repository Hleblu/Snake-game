#pragma once
#include "constants.h"
#include "apple.h"
#include <deque>

class Game 
{
    sf::SoundBuffer eatSoundBuffer, gameOverSoundBuffer, moveSoundBuffer;
    sf::Clock clock;
    sf::VertexArray backgroundVertices;
    sf::Sound someSound;
    std::deque<sf::Sound> soundsArray;
    Snake snake;
    Apple apple;
    double time = 0, timer = 0, delay = 0.115;
    void playSound(sf::SoundBuffer& buffer);
    void createBackground();
public:
    void start(sf::RenderWindow& window);
    Game();
};