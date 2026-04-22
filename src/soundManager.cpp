#include "soundManager.hpp"

void SoundManager::addSound(const std::string& name, const void* data, std::size_t dataSize) {
    sf::SoundBuffer dummyBuffer;
    if (!dummyBuffer.loadFromMemory(data, dataSize)) {
        throw std::runtime_error("couldn't load sound: " + name);
    }
    else {
        soundsMap.emplace(name, std::move(dummyBuffer));
    }
}

void SoundManager::playSound(const std::string& soundName) {
    if (soundsMap.find(soundName) != soundsMap.end()) {
        auto& buffer = soundsMap[soundName];

        for (auto& sound : soundsPool) {
            if (sound.getStatus() == sf::Sound::Status::Stopped) {
                sound.setBuffer(buffer);
                sound.play();
                return;
            }
        }

        auto oldestSound = std::min_element(
            soundsPool.begin(),
            soundsPool.end(),
            [](const sf::Sound& a, const sf::Sound& b) {
                return a.getPlayingOffset() > b.getPlayingOffset();
            });

        oldestSound->setBuffer(buffer);
        oldestSound->play();
    }
}

SoundManager::SoundManager(const int bufferSize) {
    if (bufferSize < 0)
        throw std::invalid_argument("buffer size of SoundManager instance can't be negative");

    sf::SoundBuffer dummy;
    soundsPool.resize(bufferSize, sf::Sound(dummy));
}