#pragma once
#include "state.h"
#include <functional>
#include <memory>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuItem : public sf::Drawable
{
	std::function<void(MenuItem&)> callback;
	sf::Text text;
	bool interactive = false;
public:
	MenuItem(const std::string& label, sf::Font& font, unsigned int fontSize);

	void setLabel(const std::string& label);
	void setColor(const sf::Color& color);
	void setOutlineColor(const sf::Color& color);
	void setCallback(std::function<void(MenuItem&)> newCallback);
	bool checkBoundaries(const sf::Vector2f& position) const;
	bool isInteractive() const;
	void callFunction();
	void setPosition(const sf::Vector2f& position);
	float getSpacing() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class Menu 
{
	std::vector<std::unique_ptr<MenuItem>> items;
	State& currentState;
	State thisState;

	void draw(sf::RenderWindow& window);
	MenuItem* updateHover(const sf::Vector2f& mousePos);
public:
	Menu(State& curr, State self);
	MenuItem& addItem(std::unique_ptr<MenuItem> item);
	MenuItem& addItem(const std::string& label, sf::Font& font, unsigned int fontSize);
	void build();
	void show(sf::RenderWindow& window);
};
