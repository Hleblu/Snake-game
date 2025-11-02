#include "configuration.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "randomGenerator.hpp"
#include "Resources/tiny5_regular.hpp"
#include "Resources/game_icon.hpp"
#include "state.h"
#include <cstdio>
#include <SFML/Graphics/Image.hpp>

int main()
{
    try {
        RandomGenerator::seed(static_cast<uint64_t>(std::time(nullptr)));

        State state;

        Game game;
        sf::RenderWindow window(sf::VideoMode(
            { config.width, config.height }),
            "Snake",
            sf::Style::Titlebar | sf::Style::Close
        );
        window.setKeyRepeatEnabled(false);
        window.setVerticalSyncEnabled(true);

        sf::Image icon;
        if (!icon.loadFromMemory(game_icon, game_icon_len)) throw std::runtime_error("Can\'t load icon");
        window.setIcon({ icon.getSize().x, icon.getSize().y }, icon.getPixelsPtr());

        sf::Font tiny5;

        if (!tiny5.openFromMemory(tiny5_regular, tiny5_regular_len))
            throw std::runtime_error("failed to load font from memory");

        Menu menu(state, State::MENU);

        menu.addItem("Snake Game", tiny5, 96);

        menu.addItem("Start", tiny5, 84)
            .setCallback([&](auto& self) { state = State::GAME; });

        menu.addItem("Settings", tiny5, 84)
            .setCallback([&](auto& self) { state = State::SETTINGS; });

        menu.addItem("Exit", tiny5, 84)
            .setCallback([&](auto& self) { state = State::EXIT; });

        menu.build();

        Menu settings(state, State::SETTINGS);

        settings.addItem("Settings", tiny5, 96);

        settings.addItem("Speed: Default", tiny5, 72)
            .setCallback([](auto& self) {
                config.cycleSpeed();
                self.setLabel(config.getCurrentSpeedLabel());
            });

        settings.addItem("Field size: Default", tiny5, 72)
            .setCallback([](auto& self) {
                config.cycleGridSize();
                self.setLabel(config.getCurrentGridLabel());
            });

        settings.addItem("Obstacles: Enabled", tiny5, 72)
            .setCallback([](auto& self) {
                config.toggleObstacles();
                self.setLabel(config.getCurrentObstaclesLabel());
            });

        settings.addItem("Change theme", tiny5, 72)
            .setCallback([](auto& self) {
                config.cycleTheme();
            });

        settings.addItem("Go back", tiny5, 72)
            .setCallback([&](auto& self) { state = State::MENU; });

        settings.build();

        state = State::MENU;
        while (window.isOpen() && state != State::EXIT) {
            switch (state) {
                case State::MENU:
                    menu.show(window);
                    break;
                case State::SETTINGS:
                    settings.show(window);
                    break;
                case State::GAME:
                    game.start(window);
                    state = State::MENU;
                    break;
                default:
                    state = State::EXIT;
                    break;
            }
        }
    }

    catch (const std::exception& e){
        fprintf(stderr, "!!!!!: %s\n", e.what());
    }
}
