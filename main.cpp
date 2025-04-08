#include "menu.h"
#include "game.h"
#include "Resources/icon.c"

void changeSpeed(Game& game, Menu& menu)
{
    static std::vector<std::pair<std::string, float>> speedOptions = {
        {"Speed: Standart", 0.115},
        {"Speed: Fast", 0.090},
        {"Speed: Slow", 0.140}
    };
    static int currentIndex = 0;
    currentIndex = (currentIndex + 1) % speedOptions.size();
    menu.changeText(speedOptions[currentIndex].first, 1);
    game.changeDelay(speedOptions[currentIndex].second);
}

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
    menu.createItem("Start", [&game, &window]() { game.start(window); });
    menu.createItem("Speed: Standart", [&game, &menu]() { changeSpeed(game, menu); });
    menu.createItem("Exit", [&window]() { window.close(); });
    menu.showMenu(window);

    return 0;
}
