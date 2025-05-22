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
        menu.createItem("Exit", [&window]() { window.close(); });
        menu.showMenu(window);
    }

    catch (const std::exception& e){
        std::cerr << "!!!!!:" << e.what();
    }
}
