#pragma once
#include <map>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class SoundManager {
    std::vector<sf::Sound> soundsPool;
    std::map<std::string, sf::SoundBuffer> soundsMap;

public:
    void addSound(const std::string& name, const void* data, std::size_t dataSize);
    void playSound(const std::string& soundName);

    SoundManager(const int bufferSize);
};