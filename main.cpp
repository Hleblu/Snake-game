#include "menu.h"
#include "game.h"
#include "configuration.h"
#include "Resources/icon.c"
#include <iostream>

int main()
{
    try {
        Configuration* config = Configuration::getInstance();
        Game game;
        sf::RenderWindow window(sf::VideoMode({ config->width, config->height }), "Snake", sf::Style::Titlebar | sf::Style::Close);
        window.setKeyRepeatEnabled(false);
        window.setVerticalSyncEnabled(true);

        sf::Image icon;
        if (!icon.loadFromMemory(snake_png, snake_png_len)) throw std::runtime_error("Can\'t load icon");
        window.setIcon({ icon.getSize().x, icon.getSize().y }, icon.getPixelsPtr());

        Menu menu;
        menu.setTitle("SNAKE GAME");
        menu.createItem("Start", [&game, &window]() { game.start(window); });

        Menu settings;
        settings.setTitle("SETTINGS");

        settings.createItem("Speed: Default", [&config, &settings]() {
            static int index = 0;
            config->cycleOptions(config->delay, config->speedOptions, index);
            settings.setItemLabel(0, config->speedOptionsLabels[index]);
            });

        settings.createItem("Field size: Default", [&config, &settings]() {
            static int index = 0;
            config->cycleOptions(config->size, config->cellSizes, index);
            config->rows = config->width / config->size;
            config->columns = config->height / config->size;
            settings.setItemLabel(1, config->fieldSizeOptionsLabels[index]);
            });

        settings.createItem("Obstacles: Enabled", [&config, &settings]() {
            static int index = 0;
            config->cycleOptions(config->obstaclesEnabled, config->boolOptions, index);
            settings.setItemLabel(2, config->obstaclesOptionsLabels[index]);
            });

        settings.createItem("Change color theme", [&config]() {
            static int index = 0;
            config->cycleOptions(config->currentTheme, config->themes, index);
            });

        settings.createItem("Go back", [&settings]() { settings.setMenuActive(false); });
        menu.createItem("Settings", [&settings, &window]() { settings.showMenu(window); });

        menu.createItem("Exit", [&menu]() { menu.setMenuActive(false); });
        menu.showMenu(window);
    }

    catch (const std::exception& e){
        std::cerr << "!!!!!:" << e.what();
    }
}
