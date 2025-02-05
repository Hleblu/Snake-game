#include "menu.h"

Menu::Menu() : selectedItem(0)
{
    mainFont.loadFromFile("Resources/Tiny5-Regular.ttf");

    title.setFillColor(sf::Color::White);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Black);
    title.setCharacterSize(width / 8);
    title.setFont(mainFont);
}

void Menu::setTitle(const std::string& titleText)
{
    title.setString(titleText);
    title.setPosition(width / 10, height / 5);
}

void Menu::createItem(const std::string& label, std::function<void(sf::RenderWindow&)> action)
{
    sf::Text button;
    button.setFont(mainFont);
    button.setFillColor(sf::Color::White);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::Black);
    button.setCharacterSize(fontSize);
    button.setString(label);
    button.setPosition(width / 10, height / 3 + fontSize * items.size());
    items.push_back({ button, action });
}

void Menu::drawItems(sf::RenderWindow& window)
{
    window.draw(title);
    for (auto& i : items) {
        window.draw(i.button);
    }
}

void Menu::showMenu(sf::RenderWindow& window)
{
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            mousePos = sf::Mouse::getPosition(window);

            for (int i = 0; i < items.size();++i) {
                if (items[i].button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (selectedItem != i) {
                        selectedItem = i;
                    }
                    items[selectedItem].button.setFillColor(sf::Color(172, 206, 94));
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        items[selectedItem].action(window);
                    }
                    break;
                }
                if (items[selectedItem].button.getFillColor() != sf::Color::White) items[selectedItem].button.setFillColor(sf::Color::White);
            }
        }

        window.clear(sf::Color(114, 183, 106));
        drawItems(window);
        window.display();
    }
}