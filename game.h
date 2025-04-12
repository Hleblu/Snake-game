#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "apple.h"
#include "snake.h"
#include "configuration.h"
#include "renderer.h"
#include "Resources/Sounds/sound_food.c"
#include "Resources/Sounds/sound_move.c"
#include "Resources/Sounds/sound_gameover.c"

class Game 
{
    sf::SoundBuffer eatSoundBuffer, gameOverSoundBuffer, moveSoundBuffer;
    sf::Clock clock;
    sf::Sound someSound;
    std::deque<sf::Sound> soundsArray;
    Snake snake;
    Apple apple;
    Configuration* config = Configuration::getInstance();
	Renderer* renderer = Renderer::getInstance();
    bool isGameOver = false;

    void playSound(sf::SoundBuffer& buffer);
	void clearSoundsArray();
    void restoreDefaults();
public:
    void start(sf::RenderWindow& window);
    Game();
};