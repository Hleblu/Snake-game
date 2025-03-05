#include "menu.h"
#include "game.h"
#include "Resources/icon.c"

int main()
{
    Game game;
    sf::RenderWindow window(sf::VideoMode({ width, height }), "Snake", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(true);

    sf::Image icon;
    if(!icon.loadFromMemory(snake_png, snake_png_len)) return -1;
    window.setIcon({ icon.getSize().x, icon.getSize().y }, icon.getPixelsPtr());

    Menu menu;
    menu.reserveSpace(3);
    menu.setTitle("SNAKE GAME");
    menu.createItem("Start", [&game, &window](sf::RenderWindow&) { game.start(window); });
    menu.createItem("Exit", [&window](sf::RenderWindow&) { window.close(); });
    menu.showMenu(window);

    return 0;
}
