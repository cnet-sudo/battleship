#pragma once

/**
 * @enum ShotResult
 * @brief Результат выстрела по клетке.
 */
enum class ShotResult : unsigned char {
    Invalid,    ///< координаты вне поля
    Repeat,     ///< повторный выстрел
    Miss,       ///< промах
    Hit,        ///< попадание
    Sunk        ///< корабль уничтожен
};
