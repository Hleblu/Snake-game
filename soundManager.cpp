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

        for (auto& sound : soundsBuffer) {
            if (sound.getStatus() == sf::Sound::Status::Stopped) {
                sound.setBuffer(buffer);
                sound.play();
                return;
            }
        }

        auto oldestSound = std::min_element(
            soundsBuffer.begin(),
            soundsBuffer.end(),
            [](const sf::Sound& a, const sf::Sound& b) {
                return a.getPlayingOffset() > b.getPlayingOffset();
            });

        oldestSound->setBuffer(buffer);
        oldestSound->play();
    }
}

SoundManager::SoundManager(const int bufferSize) {
    sf::SoundBuffer dummy;
    soundsBuffer.resize(bufferSize, sf::Sound(dummy));
}