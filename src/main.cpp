#include "configuration.hpp"
#include "difficultyManager.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "pathUtils.hpp"
#include "randomGenerator.hpp"
#include "icons/game_icon.hpp"
#include "fonts/tiny5_regular.hpp"
#include "saveManager.hpp"  
#include "state.hpp"
#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio/Listener.hpp>

void showLoadingScreen(sf::RenderWindow& window, sf::Font& font, Configuration& config)
{
    sf::Text loadingTip(font, "Loading..", 72);
    const sf::FloatRect bounds = loadingTip.getLocalBounds();
    loadingTip.setOrigin(bounds.position + bounds.size / 2.f);
    loadingTip.setPosition(config.getFieldDimensions<float>() / 2.f);

    window.clear(sf::Color::Black);
    window.draw(loadingTip);
    window.display();
}

int main()
{
    Configuration config;
    DifficultyManager difficulty;

    try {
        const std::string savePath = PathUtils::getSaveFilePath("Snake", "SnakeSave");
        SaveManager saveManager;
        saveManager.bind(config);
        saveManager.bind(difficulty);
        saveManager.load(savePath);

        sf::RenderWindow window(sf::VideoMode(
            config.getFieldDimensions<unsigned int>()),
            "Snake",
            sf::Style::Default //^ sf::Style::Resize
        );
        window.setMinimumSize(config.getFieldDimensions<unsigned int>() / 2u);
        window.setKeyRepeatEnabled(false);
        window.setFramerateLimit(160);

        sf::Image icon;
        if (!icon.loadFromMemory(game_icon, game_icon_len)) throw std::runtime_error("Can\'t load icon");
        window.setIcon({ icon.getSize() }, icon.getPixelsPtr());

        sf::Font tiny5;
        tiny5.setSmooth(false);

        if (!tiny5.openFromMemory(tiny5_regular, tiny5_regular_len))
            throw std::runtime_error("failed to load font from memory");

        showLoadingScreen(window, tiny5, config);

        RandomGenerator::seed(static_cast<uint64_t>(std::time(nullptr)));

        State state;

        Game game(config, tiny5, difficulty);

        Menu menu(state, State::MENU, config);

        menu.addItem("Snake Game", tiny5, 96);

        menu.addItem("Start", tiny5, 80)
            .setCallback([&](auto& self) { state = State::GAME; });

        menu.addItem("Settings", tiny5, 80)
            .setCallback([&](auto& self) { state = State::SETTINGS; });

        menu.addItem("Exit", tiny5, 80)
            .setCallback([&](auto& self) { state = State::EXIT; });

        menu.build();

        Menu settings(state, State::SETTINGS, config);

        settings.addItem("Settings", tiny5, 96);

        settings.addItem(config.getCurrentSpeedLabel(), tiny5, 72)
            .setCallback([&](auto& self) {
                config.cycleSpeed();
                self.setString(config.getCurrentSpeedLabel());
            });

        settings.addItem(config.getCurrentGridLabel(), tiny5, 72)
            .setCallback([&](auto& self) {
                config.cycleGridSize();
                self.setString(config.getCurrentGridLabel());
            });

        settings.addItem(config.getCurrentObstaclesLabel(), tiny5, 72)
            .setCallback([&](auto& self) {
                config.toggleObstacles();
                self.setString(config.getCurrentObstaclesLabel());
            });

        settings.addItem("Next", tiny5, 72)
            .setCallback([&](auto& self) { state = State::SETTINGS_SECOND; });

        settings.addItem("Go back", tiny5, 72)
            .setCallback([&](auto& self) { state = State::MENU; });

        settings.build();

        Menu settingsSecond(state, State::SETTINGS_SECOND, config);

        settingsSecond.addItem("Settings: 2", tiny5, 96);

        settingsSecond.addItem(config.getCurrentSoundLabel(), tiny5, 72)
            .setCallback([&](auto& self) {
                config.toggleSound();
                const float listenerValue = config.isSoundEnabled() ? 100.f : 0.f;
                sf::Listener::setGlobalVolume(listenerValue);
                self.setString(config.getCurrentSoundLabel());
            });

        settingsSecond.addItem(config.getCurrentShakeLabel(), tiny5, 72)
            .setCallback([&](auto& self) {
                config.toggleShake();
                self.setString(config.getCurrentShakeLabel());
            });

        settingsSecond.addItem("Change theme", tiny5, 72)
            .setCallback([&](auto& self) {
                config.cycleTheme();
            });

        settingsSecond.addItem("Go back", tiny5, 72)
            .setCallback([&](auto& self) { state = State::SETTINGS; });

        settingsSecond.build();

        state = State::MENU;
        while (window.isOpen() && state != State::EXIT) {
            switch (state) {
                case State::MENU:
                    menu.show(window);
                    break;
                case State::SETTINGS:
                    settings.show(window);
                    break;
                case State::SETTINGS_SECOND:
                    settingsSecond.show(window);
                    break;
                case State::GAME:
                    game.start(window);
                    state = State::MENU;
                    break;
                default:
                    state = State::EXIT;
                    break;
            }

            saveManager.save(savePath);
        }
    }

    catch (const std::exception& e){
        std::cerr << "Error: " << e.what();
    }
}
