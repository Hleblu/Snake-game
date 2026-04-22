#pragma once

/*
* ==== Maybe in better future ===
namespace sf {
    class RenderWindow;
}

class IState
{
public:
    virtual ~IState() = default;
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual State update(sf::RenderWindow& window) = 0;
};
*/

enum class State {
    GAME,
    MENU,
    SETTINGS,
    EXIT
};