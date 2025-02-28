#pragma once
#include "constants.h"
#include "apple.h"

class Game 
{
    sf::SoundBuffer eatSoundBuffer, gameOverSoundBuffer, moveSoundBuffer;
    sf::Clock clock;
    sf::Sound someSound;
    std::deque<sf::Sound> soundsArray;
	sf::Texture backgroundTexture;
    Snake snake;
    Apple apple;
    double time = 0, timer = 0, delay = 0.115;
    void playSound(sf::SoundBuffer& buffer);
	void clearSoundsArray();
    void initializeBackground();
public:
    void start(sf::RenderWindow& window);
    Game();
};