#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "apple.h"
#include "snake.h"
#include "configuration.h"
#include "Resources/Sounds/sound_food.c"
#include "Resources/Sounds/sound_move.c"
#include "Resources/Sounds/sound_gameover.c"

class Game 
{
    sf::SoundBuffer eatSoundBuffer, gameOverSoundBuffer, moveSoundBuffer;
    sf::Clock clock;
    sf::Sound someSound;
    std::deque<sf::Sound> soundsArray;
	sf::Texture backgroundTexture;
    Snake snake;
    Apple apple;
    Configuration* config;
    bool isGameOver = false;

    void playSound(sf::SoundBuffer& buffer);
	void clearSoundsArray();
    void initializeBackground();
    void restoreDefaults();
public:
    void start(sf::RenderWindow& window);
    Game(Configuration& config);
};