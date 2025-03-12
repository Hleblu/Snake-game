#pragma once
#include "constants.h"
#include "apple.h"
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
    sf::Color mainColor, secondColor;
    float time = 0, timer = 0, animationTimer = 0, delay = 0.115;

    bool isGameOver = false;
    void playSound(sf::SoundBuffer& buffer);
	void clearSoundsArray();
    void initializeBackground();
    void restoreDefaults();
public:
    void changeDelay(float d);
    void start(sf::RenderWindow& window);
    Game();
};