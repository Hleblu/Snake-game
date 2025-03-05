#include "menu.h"

Menu::Menu() : title(mainFont)
{
    if(!mainFont.openFromMemory(Tiny5_Regular_ttf, Tiny5_Regular_ttf_len)) return;

    title.setFillColor(sf::Color::White);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Black);
    title.setCharacterSize(width / 8);
    title.setFont(mainFont);

    mainColor = { 114, 183, 106 };
    secondColor = { 172, 206, 94 };
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

            for (auto& item : items) {
                if (item.button.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                    if (item.button.getFillColor() == sf::Color::White) item.button.setFillColor(secondColor);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        item.action(window);
                    }
                    break;
                }
                else if (item.button.getFillColor() != sf::Color::White) item.button.setFillColor(sf::Color::White);
            }
        }

        window.clear(mainColor);
        drawItems(window);
        window.display();
    }
}