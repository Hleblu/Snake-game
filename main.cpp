#include "menu.h"
#include "game.h"

int main()
{
    Game game;
    sf::RenderWindow window(sf::VideoMode(width, height), "Snake", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(true);

    sf::Image icon;
    icon.loadFromFile("Resources/Snake.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    Menu menu;
    menu.setTitle("SNAKE GAME");
    menu.createItem("Start", [&game, &window](sf::RenderWindow&) { game.Start(window); });
    menu.createItem("Exit", [&window](sf::RenderWindow&) { window.close(); });
    menu.showMenu(window);

    return 0;
}
