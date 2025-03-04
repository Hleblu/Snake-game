#include "menu.h"

Menu::Menu() : title(mainFont)
{
    if(!mainFont.openFromMemory(Tiny5_Regular_ttf, Tiny5_Regular_ttf_len)) return;

    title.setFillColor(sf::Color::White);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Black);
    title.setCharacterSize(width / 8);
    title.setFont(mainFont);
}

void Menu::reserveSpace(int size) {
    items.reserve(size);
}

void Menu::setTitle(const std::string& titleText)
{
    title.setString(titleText);
    title.setPosition({ width / 10, height / 5 });
}

void Menu::createItem(const std::string& label, std::function<void(sf::RenderWindow&)> action)
{
    sf::Text button(mainFont);
    button.setFillColor(sf::Color::White);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::Black);
    button.setCharacterSize(fontSize);
    button.setString(label);
    button.setPosition({ width / 10, height / 3 + fontSize * static_cast<float>(items.size()) });
    items.emplace_back(std::move(button), std::move(action));
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
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if(event->is<sf::Event::MouseMoved>()) mousePos = sf::Mouse::getPosition(window);

            for (int i = 0; i < items.size();++i) {
                if (items[i].button.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                    items[i].button.setFillColor(sf::Color(172, 206, 94));
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        items[i].action(window);
                    }
                    break;
                }
                if (items[i].button.getFillColor() != sf::Color::White) items[i].button.setFillColor(sf::Color::White);
            }
        }

        window.clear(sf::Color(114, 183, 106));
        drawItems(window);
        window.display();
    }
}