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

/*
 Board
 Игровое поле для Морского боя.
 
 Хранит состояние клеток, размещает корабли, обрабатывает выстрелы
 и определяет затопление кораблей.
 */
class Board {
public:
    Board();

    /*
     Полная очистка поля.
     */
    void reset() noexcept;

    /*
     Проверка выхода за границы.
     */
    [[nodiscard]]
    bool isInside(int x, int y) const noexcept;

    /*
     Случайная расстановка флота.
     */
    void randomPlaceFleet(std::mt19937& rng);

    /*
     Выстрел по клетке.
     ShotResult — результат выстрела.
     */
    [[nodiscard]]
    ShotResult shoot(int x, int y) noexcept;

    /*
     Проверка уничтожения всего флота.
     */
    [[nodiscard]]
    bool allShipsDestroyed() const noexcept;

    /*
     Доступ к сетке поля.
     */
    [[nodiscard]]
    const BoardGrid& cells() const noexcept { return cells_; }

private:
    BoardGrid cells_{};          ///< состояние клеток
    int shipsCellsTotal_ = 0;    ///< всего клеток кораблей
    int shipsCellsHit_ = 0;      ///< попаданий по кораблям

    /*
     Проверка возможности поставить корабль.
     
     Правило: корабли не соприкасаются ни сторонами, ни углами.
     */
    [[nodiscard]]
    bool canPlaceShip(int x, int y, int length, bool horizontal) const noexcept;

    /*
     Установка корабля (без проверки).
     */
    void placeShip(int x, int y, int length, bool horizontal) noexcept;

    /*
     Проверка, затоплен ли корабль.
     */
    [[nodiscard]]
    bool isShipSunk(int x, int y) const noexcept;

    /*
     Пометить весь корабль как затопленный.
     */
    void markShipSunk(int x, int y) noexcept;
};
