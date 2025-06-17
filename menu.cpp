    #include "menu.h"

    Menu::Menu() : title(mainFont)
    {
        fontSize = config.width * 0.05f + config.height * 0.05f;
        if(!mainFont.openFromMemory(Tiny5_Regular_ttf, Tiny5_Regular_ttf_len))
            throw ("failed to load font from memory");

        const unsigned int titleFontSize = fontSize * 1.25f;
        title.setCharacterSize(titleFontSize);
        title.setOutlineThickness(titleFontSize * 0.055f);
        title.setFillColor(config.textColor1);
        title.setOutlineColor(config.textColor2);
        title.setFont(mainFont);
    }

    void Menu::setTitle(const std::string& titleText)
    {
        title.setString(titleText);
        title.setPosition({config.width * 0.1f, config.height * 0.16f });
    }

    void Menu::createItem(const std::string& label, std::function<void()> action)
    {
        const float spacing = fontSize * 0.5f;
        const float startY = title.getPosition().y + title.getGlobalBounds().size.y;
        const float posY = startY + spacing + fontSize * items.size();
        const float posX = config.width * 0.1f;

        sf::Text button(mainFont, label, fontSize);
        button.setFillColor(config.textColor1);
        button.setOutlineColor(config.textColor2);
        button.setOutlineThickness(fontSize * 0.055f);
        button.setPosition({ posX, posY });

        items.push_back({ button, action });
    }

    void Menu::drawItems(sf::RenderWindow& window)
    {
        window.draw(title);
        for (auto& item : items) {
            window.draw(item.button);
        }
    }

    void Menu::setMenuActive(const bool state) {
        menuIsActive = state;
    }

    void Menu::setItemLabel(const unsigned int index, const std::string& label) {
        if (index < items.size()) {
            items.at(index).setLabel(label);
        }
    }

    void Menu::resetHover() {
        for (auto& item : items)
            item.button.setFillColor(config.textColor1);
    }

    void Menu::showMenu(sf::RenderWindow& window)
    {
        menuIsActive = true;
        sf::Vector2f mousePos;
        while (window.isOpen() && menuIsActive) {
            mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();

                const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
                const bool isClicked = mouseButtonPressed && mouseButtonPressed->button == sf::Mouse::Button::Left;

                for (auto& item : items) {
                    const bool isHovered = item.button.getGlobalBounds().contains({ mousePos.x, mousePos.y });

                    if (isHovered) {

                        if (isClicked) {
                            item.action();
                        }

                        item.button.setFillColor(config.currentTheme->secondColor);
                    }
                    else item.button.setFillColor(config.textColor1);
                }
            }

            window.clear(config.currentTheme->mainColor);
            drawItems(window);
            window.display();
        }
        resetHover();
    }