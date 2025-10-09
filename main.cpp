#include "configuration.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "randomGenerator.hpp"
#include "Resources/icon.c"
#include <cstdio>
#include <SFML/Graphics/Image.hpp>

enum GameState {
    GAME,
    MENU,
    SETTINGS,
    EXIT
} state, nextState;

int main()
{
    try {
        RandomGenerator::seed(static_cast<uint64_t>(std::time(nullptr)));

        Game game;
        sf::RenderWindow window(sf::VideoMode({ config.width, config.height }), "Snake", sf::Style::Titlebar | sf::Style::Close);
        window.setKeyRepeatEnabled(false);
        window.setVerticalSyncEnabled(true);

        sf::Image icon;
        if (!icon.loadFromMemory(snake_png, snake_png_len)) throw std::runtime_error("Can\'t load icon");
        window.setIcon({ icon.getSize().x, icon.getSize().y }, icon.getPixelsPtr());

        Menu menu;
        menu.setTitle("SNAKE GAME");

        menu.createItem("Start", [&menu]() { 
            nextState = GameState::GAME;
            menu.setMenuActive(false);
            });

        menu.createItem("Settings", [&menu]() {
            nextState = GameState::SETTINGS;
            menu.setMenuActive(false);
            });

        menu.createItem("Exit", [&menu]() {
            nextState = GameState::EXIT;
            menu.setMenuActive(false);
            });

        Menu settings;
        settings.setTitle("SETTINGS");

        settings.createItem("Speed: Default", [&settings]() {
            static int index = 0;
            config.cycleOptions(config.delay, config.speedOptions, index);
            settings.setItemLabel(0, config.speedOptionsLabels[index]);
            });

        settings.createItem("Field size: Default", [&settings]() {
            static int index = 0;
            config.cycleOptions(config.size, config.cellSizes, index);
            config.rows = config.width / config.size;
            config.columns = config.height / config.size;
            settings.setItemLabel(1, config.fieldSizeOptionsLabels[index]);
            });

        settings.createItem("Obstacles: Enabled", [&settings]() {
            static int index = 0;
            config.cycleOptions(config.obstaclesEnabled, config.boolOptions, index);
            settings.setItemLabel(2, config.obstaclesOptionsLabels[index]);
            });

        settings.createItem("Change color theme", []() {
            static int index = 0;
            config.cycleOptions(config.currentTheme, config.themes, index);
            });

        settings.createItem("Go back", [&settings]() { 
            nextState = GameState::MENU;
            settings.setMenuActive(false); 
            });

        state = GameState::MENU;
        nextState = state;
        while (window.isOpen() && state != GameState::EXIT) {
            switch (state) {
                case GameState::MENU:
                    menu.showMenu(window);
                    state = nextState;
                    break;
                case GameState::SETTINGS:
                    settings.showMenu(window);
                    state = nextState;
                    break;
                case GameState::GAME:
                    game.start(window);
                    state = GameState::MENU;
                    break;
                default:
                    state = GameState::EXIT;
                    break;
            }
        }
    }

    catch (const std::exception& e){
        fprintf(stderr, "!!!!!: %s\n", e.what());
    }
}
