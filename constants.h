#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Resources/Sounds/sound_food.c"
#include "Resources/Sounds/sound_move.c"
#include "Resources/Sounds/sound_gameover.c"
#include "Resources/font.c"
#include "Resources/icon.c"

constexpr int size = 40;
constexpr int gridX = 17;
constexpr int gridY = 15;
constexpr int width = gridX * size;
constexpr int height = gridY * size;