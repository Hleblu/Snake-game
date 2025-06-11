#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "apple.h"
#include "snake.h"
#include "configuration.h"
#include "renderer.h"
#include "collisionManager.h"
#include "obstacle.h"
#include "Resources/Sounds/sound_food.c"
#include "Resources/Sounds/sound_move.c"
#include "Resources/Sounds/sound_gameover.c"

class Game 
{
    sf::SoundBuffer eatSoundBuffer, gameOverSoundBuffer, moveSoundBuffer;
    std::vector<sf::Sound> soundsArray;
    void playSound(sf::SoundBuffer& buffer);

    sf::Clock clock;
    Snake snake;
    Obstacle obstacle;
    std::unique_ptr<Apple> apple = AppleFactory::createRandomApple();
    Configuration* config = Configuration::getInstance();
	Renderer* renderer = Renderer::getInstance();
    CollisionManager* collisionManager = CollisionManager::getInstance();

    void restoreDefaults();
public:
    void start(sf::RenderWindow& window);
    Game();
};