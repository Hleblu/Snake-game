#include "configuration.hpp"
#include "game.hpp"
#include "menu.hpp"
#include "pathUtils.hpp"
#include "randomGenerator.hpp"
#include "icons/game_icon.hpp"
#include "fonts/tiny5_regular.hpp"
#include "saveManager.hpp"
#include "state.hpp"
#include <iostream>
#include <SFML/Graphics/Image.hpp>

void showLoadingScreen(sf::RenderWindow& window, sf::Font* font, Configuration* config)
{
    sf::Text loadingTip(*font, "Loading..", 72);
    const sf::FloatRect bounds = loadingTip.getLocalBounds();
    loadingTip.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
    loadingTip.setPosition({ config->width / 2.f, config->height / 2.f });

    window.clear(sf::Color::Black);
    window.draw(loadingTip);
    window.display();
}

int main()
{
    auto config = std::make_unique<Configuration>();

    try {
        const std::string savePath = PathUtils::getSaveFilePath("Snake", "SnakeSave");
        SaveManager saveManager;
        saveManager.bind(*config);
        saveManager.load(savePath);

        sf::RenderWindow window(sf::VideoMode(
            { config->width, config->height }),
            "Snake",
            sf::Style::Titlebar | sf::Style::Close
        );
        window.setKeyRepeatEnabled(false);
        window.setFramerateLimit(160);

        sf::Image icon;
        if (!icon.loadFromMemory(game_icon, game_icon_len)) throw std::runtime_error("Can\'t load icon");
        window.setIcon({ icon.getSize().x, icon.getSize().y }, icon.getPixelsPtr());

        auto tiny5 = std::make_unique<sf::Font>();

        if (!tiny5->openFromMemory(tiny5_regular, tiny5_regular_len))
            throw std::runtime_error("failed to load font from memory");

        showLoadingScreen(window, tiny5.get(), config.get());

        RandomGenerator::seed(static_cast<uint64_t>(std::time(nullptr)));

        State state;

        Game game(config.get(), tiny5.get());

        Menu menu(state, State::MENU, config.get());

        menu.addItem("Snake Game", *tiny5, 96);

        menu.addItem("Start", *tiny5, 84)
            .setCallback([&](auto& self) { state = State::GAME; });

        menu.addItem("Settings", *tiny5, 84)
            .setCallback([&](auto& self) { state = State::SETTINGS; });

        menu.addItem("Exit", *tiny5, 84)
            .setCallback([&](auto& self) { state = State::EXIT; });

        menu.build();

        Menu settings(state, State::SETTINGS, config.get());

        settings.addItem("Settings", *tiny5, 96);

        settings.addItem(config->getCurrentSpeedLabel(), *tiny5, 72)
            .setCallback([&](auto& self) {
                config->cycleSpeed();
                self.setLabel(config->getCurrentSpeedLabel());
            });

        settings.addItem(config->getCurrentGridLabel(), *tiny5, 72)
            .setCallback([&](auto& self) {
                config->cycleGridSize();
                self.setLabel(config->getCurrentGridLabel());
            });

        settings.addItem(config->getCurrentObstaclesLabel(), *tiny5, 72)
            .setCallback([&](auto& self) {
                config->toggleObstacles();
                self.setLabel(config->getCurrentObstaclesLabel());
            });

        settings.addItem("Change theme", *tiny5, 72)
            .setCallback([&](auto& self) {
                config->cycleTheme();
            });

        settings.addItem("Go back", *tiny5, 72)
            .setCallback([&](auto& self) { state = State::MENU; });

        settings.build();

        state = State::MENU;
        while (window.isOpen() && state != State::EXIT) {
            switch (state) {
                case State::MENU:
                    menu.show(window);
                    break;
                case State::SETTINGS:
                    settings.show(window);
                    break;
                case State::GAME:
                    game.start(window);
                    state = State::MENU;
                    break;
                default:
                    state = State::EXIT;
                    break;
            }

            saveManager.save(savePath);
        }
    }

    catch (const std::exception& e){
        std::cerr << "Error: " << e.what();
    }
}
