#pragma once
#include "apple.hpp"
#include "obstacle.hpp"
#include "snake.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

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