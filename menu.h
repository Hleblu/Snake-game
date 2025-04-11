#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include <functional>
#include "Resources/font.c"

class Menu 
{
	struct menuItem {
		sf::Text button;
		std::function<void()> action;

		menuItem(sf::Text button, std::function<void()> action) : button(button), action(action) {}
	};

	std::vector<menuItem> items;
	std::string label;
	sf::Font mainFont;
	sf::Text title;
	unsigned int fontSize;
	Configuration* config;

	void drawItems(sf::RenderWindow& window);
public:
	Menu(Configuration& config);
	void changeText(const std::string& label, int index);
	void createItem(const std::string& label, std::function<void()> action);
	void setTitle(const std::string& title);
	void showMenu(sf::RenderWindow& window);
	void reserveSpace(int size);
};
