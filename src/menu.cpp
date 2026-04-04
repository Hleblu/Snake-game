#include "configuration.hpp"
#include "menu.hpp"
#include "viewUtils.hpp"

Menu::Menu(State& curr, State self, Configuration* config) : 
    currentState(curr),
    thisState(self),
    config(config)
{}

void Menu::draw(sf::RenderWindow& window)
{
    for (auto& item : items) {
        window.draw(*item);
    }
}

MenuItem* Menu::updateHover(const sf::Vector2f& mousePos)
{
    MenuItem* hoveredItem = nullptr;
    for (auto& item : items) {
        if (item->isInteractive()) {
            bool isHovered = item->checkBoundaries(mousePos);

            if (isHovered) {
                hoveredItem = item.get();
                item->setColor(config->getCurrentTheme().secondColor);
            }
            else item->setColor(config->textBaseColor);
        }
    }
    return hoveredItem;
}

MenuItem& Menu::addItem(std::unique_ptr<MenuItem> item)
{
    MenuItem* self = item.get();
    self->setColor(config->textBaseColor);
    self->setOutlineColor(config->textOutlineColor);
    items.push_back(std::move(item));
    return *self;
}

MenuItem& Menu::addItem(const std::string& label, sf::Font& font, unsigned int fontSize)
{
    auto item = std::make_unique<MenuItem>(label, font, fontSize);
    MenuItem* self = item.get();
    self->setColor(config->textBaseColor);
    self->setOutlineColor(config->textOutlineColor);
    items.push_back(std::move(item));
    return *self;
}

void Menu::build()
{
    sf::Vector2f position = { 50, 10 };
    const float offset = 5;
    for (auto& item : items) {
        item->setPosition(position);
        position.y += item->getSpacing() + offset;
    }
}

void Menu::show(sf::RenderWindow& window)
{
    ViewUtils::normalizeView(menuView, sf::Vector2f(window.getSize()));
    window.setView(menuView);

    sf::Vector2f mousePos;
    while (window.isOpen() && currentState == thisState) {

        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        auto hovered = updateHover(mousePos);

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
            const bool isClicked = mouseButtonPressed && mouseButtonPressed->button == sf::Mouse::Button::Left;

            if (hovered && isClicked) {
                hovered->callFunction();
            }
        }

        window.clear(config->getCurrentTheme().mainColor);
        draw(window);
        window.display();
    }
}

MenuItem::MenuItem(const std::string& label, sf::Font& font, unsigned int fontSize) : text(font, label, fontSize)
{
    const float thickness = fontSize * 0.07f;
    text.setOutlineThickness(thickness);
}

void MenuItem::setLabel(const std::string& label)
{
    text.setString(label);
}

void MenuItem::setColor(const sf::Color& color)
{
    text.setFillColor(color);
}

void MenuItem::setOutlineColor(const sf::Color& color)
{
    text.setOutlineColor(color);
}

void MenuItem::setCallback(std::function<void(MenuItem&)> newCallback)
{
    if (newCallback) {
        callback = newCallback;
        interactive = true;
    }
}

bool MenuItem::checkBoundaries(const sf::Vector2f& position) const
{
    return text.getGlobalBounds().contains(position);
}

bool MenuItem::isInteractive() const
{
    return interactive;
}

void MenuItem::callFunction()
{
    if (callback)
        callback(*this);
}

void MenuItem::setPosition(const sf::Vector2f& position)
{
    text.setPosition(position);
}

float MenuItem::getSpacing() const
{
    const unsigned int characterSize = text.getCharacterSize();
    return text.getFont().getLineSpacing(characterSize);
}

void MenuItem::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(text, states);
}