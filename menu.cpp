#include "menu.h"

Menu::Menu() : title(mainFont)
{
    fontSize = config->width / 20u + config->height / 20u;
    if(!mainFont.openFromMemory(Tiny5_Regular_ttf, Tiny5_Regular_ttf_len)) return;

    title.setFillColor(sf::Color::White);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Black);
    title.setCharacterSize(config->width / 8u);
    title.setFont(mainFont);
}

void Menu::reserveSpace(int size) {
    items.reserve(size);
}

void Menu::setTitle(const std::string& titleText)
{
    title.setString(titleText);
    title.setPosition({ static_cast<float>(config->width) / 10.0f, static_cast<float>(config->height) / 6.0f });
}

void Menu::createItem(const std::string& label, std::function<void()> action)
{
    sf::Text button(mainFont);
    button.setFillColor(sf::Color::White);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::Black);
    button.setCharacterSize(fontSize);
    button.setString(label);
    button.setPosition({ static_cast<float>(config->width) / 10.0f, title.getPosition().y + title.getCharacterSize() + fontSize * static_cast<float>(items.size())});

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
    sf::Vector2i mousePos;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if(event->is<sf::Event::MouseMoved>()) mousePos = sf::Mouse::getPosition(window);

            for (auto& item : items) {
                if (item.button.getGlobalBounds().contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                    if (item.button.getFillColor() == sf::Color::White) item.button.setFillColor(config->secondColor);
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
                    {
                        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                            item.action();
                        }
                    }
                    break;
                }
                else if (item.button.getFillColor() != sf::Color::White) item.button.setFillColor(sf::Color::White);
            }
        }

        window.clear(config->mainColor);
        drawItems(window);
        window.display();
    }
}