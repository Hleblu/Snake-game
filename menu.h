#pragma once
#include "constants.h"
#include <functional>

class Menu 
{
	struct menuItem {
		sf::Text button;
		std::function<void(sf::RenderWindow&)> action;

		menuItem(sf::Text button, std::function<void(sf::RenderWindow&)> action) : button(button), action(action) {}
	};

	std::vector<menuItem> items;
	std::string label;
	sf::Vector2i mousePos;
	sf::Font mainFont;
	sf::Text title;
	int fontSize = width / 20 + height / 20;

	void drawItems(sf::RenderWindow& window);
public:
	Menu();
	void createItem(const std::string& label, std::function<void(sf::RenderWindow&)> action);
	void setTitle(const std::string& title);
	void showMenu(sf::RenderWindow& window);
};
