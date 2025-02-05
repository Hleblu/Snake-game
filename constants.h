#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const int size = 40;
constexpr int width = 17 * size;
constexpr int height = 15 * size;
constexpr int gridX = width / size;
constexpr int gridY = height / size;
constexpr int vertexGrid = gridX * gridY * 6;