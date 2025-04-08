#pragma once
#include "constants.h"
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
	sf::Vector2i mousePos;
	sf::Font mainFont;
	sf::Text title;
	int fontSize = width / 20 + height / 20;
	sf::Color mainColor, secondColor;

	void drawItems(sf::RenderWindow& window);
public:
	Menu();
	void changeText(const std::string& label, int index);
	void createItem(const std::string& label, std::function<void()> action);
	void setTitle(const std::string& title);
	void showMenu(sf::RenderWindow& window);
	void reserveSpace(int size);
};
