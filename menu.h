#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include <functional>
#include "Resources/font.c"

class Menu 
{
	struct MenuItem {
		sf::Text button;
		std::function<void()> action;

		MenuItem(sf::Text button, std::function<void()> action) : button(button), action(action) {}
		void setLabel(const std::string& label) {
			button.setString(label);
		}
	};

	std::vector<MenuItem> items;
	sf::Font mainFont;
	sf::Text title;
	unsigned int fontSize;
	bool menuIsActive = false;

	void drawItems(sf::RenderWindow& window);
	void resetHover();
public:
	Menu();
	void createItem(const std::string& label, std::function<void()> action);
	void setTitle(const std::string& title);
	void showMenu(sf::RenderWindow& window);
	void setMenuActive(const bool state);
	void setItemLabel(const unsigned int index, const std::string& label);
};
