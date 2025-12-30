#pragma once

#include <cstddef>
#include <array>

/**
 * @file GameConfig.h
 * @brief Глобальные параметры игры «Морской бой».
 */

 // Размер игрового поля
inline constexpr std::size_t BOARD_SIZE = 10;

// Размер клетки в пикселях
inline constexpr int CELL_SIZE = 32;

// Отступы от края окна
inline constexpr int PADDING = 50;

// Размеры окна
inline constexpr int WINDOW_WIDTH = 900;
inline constexpr int WINDOW_HEIGHT = 500;

// Размеры кораблей (классический морской бой)
inline constexpr std::array<int, 10> SHIP_SIZES{
    4,          // линкор
    3, 3,       // крейсера
    2, 2, 2,    // эсминцы
    1, 1, 1, 1  // катера
};
