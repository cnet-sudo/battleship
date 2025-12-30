#pragma once

#include <array>
#include <random>
#include <cstddef>
#include <iostream>

#include "CellState.h"
#include "GameConfig.h"
#include "ShotResult.h"

// Удобный alias для сетки поля
using BoardGrid = std::array<std::array<CellState, BOARD_SIZE>, BOARD_SIZE>;

/**
 * @class Board
 * @brief Игровое поле для Морского боя.
 *
 * Хранит состояние клеток, размещает корабли, обрабатывает выстрелы
 * и определяет затопление кораблей.
 */
class Board {
public:
    Board();

    /**
     * @brief Полная очистка поля.
     */
    void reset() noexcept;

    /**
     * @brief Проверка выхода за границы.
     */
    [[nodiscard]]
    bool isInside(int x, int y) const noexcept;

    /**
     * @brief Случайная расстановка флота.
     */
    void randomPlaceFleet(std::mt19937& rng);

    /**
     * @brief Выстрел по клетке.
     *
     * @return ShotResult — результат выстрела.
     */
    [[nodiscard]]
    ShotResult shoot(int x, int y) noexcept;

    /**
     * @brief Проверка уничтожения всего флота.
     */
    [[nodiscard]]
    bool allShipsDestroyed() const noexcept;

    /**
     * @brief Доступ к сетке поля.
     */
    [[nodiscard]]
    const BoardGrid& cells() const noexcept { return cells_; }

private:
    BoardGrid cells_{};          ///< состояние клеток
    int shipsCellsTotal_ = 0;    ///< всего клеток кораблей
    int shipsCellsHit_ = 0;      ///< попаданий по кораблям

    /**
     * @brief Проверка возможности поставить корабль.
     *
     * Правило: корабли не соприкасаются ни сторонами, ни углами.
     */
    [[nodiscard]]
    bool canPlaceShip(int x, int y, int length, bool horizontal) const noexcept;

    /**
     * @brief Установка корабля (без проверки).
     */
    void placeShip(int x, int y, int length, bool horizontal) noexcept;

    /**
     * @brief Проверка, затоплен ли корабль.
     */
    [[nodiscard]]
    bool isShipSunk(int x, int y) const noexcept;

    /**
     * @brief Пометить весь корабль как затопленный.
     */
    void markShipSunk(int x, int y) noexcept;
};
