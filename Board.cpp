#include "Board.h"
#include <iostream>

// ------------------------------------------------------------
//  Конструктор
// ------------------------------------------------------------
Board::Board() {
    reset();
}

// ------------------------------------------------------------
//  Проверка выхода за границы
// ------------------------------------------------------------
bool Board::isInside(int x, int y) const noexcept {
    return x >= 0 && x < static_cast<int>(BOARD_SIZE) &&
        y >= 0 && y < static_cast<int>(BOARD_SIZE);
}

// ------------------------------------------------------------
//  Очистка поля
// ------------------------------------------------------------
void Board::reset() noexcept {
    for (auto& row : cells_) {
        row.fill(CellState::Empty);
    }
    shipsCellsTotal_ = 0;
    shipsCellsHit_ = 0;
}

// ------------------------------------------------------------
//  Проверка возможности поставить корабль
//  Правило: корабли не соприкасаются ни сторонами, ни углами
// ------------------------------------------------------------
bool Board::canPlaceShip(int x, int y, int length, bool horizontal) const noexcept {
    const int dx = horizontal ? 1 : 0;
    const int dy = horizontal ? 0 : 1;

    const int endX = x + dx * (length - 1);
    const int endY = y + dy * (length - 1);

    if (!isInside(endX, endY))
        return false;

    for (int i = 0; i < length; ++i) {
        const int cx = x + dx * i;
        const int cy = y + dy * i;

        // Проверяем саму клетку и окружение 3×3
        for (int ny = cy - 1; ny <= cy + 1; ++ny) {
            for (int nx = cx - 1; nx <= cx + 1; ++nx) {
                if (!isInside(nx, ny))
                    continue;

                if (cells_[ny][nx] == CellState::Ship)
                    return false;
            }
        }
    }
    return true;
}

// ------------------------------------------------------------
//  Установка корабля (без дополнительной проверки)
// ------------------------------------------------------------
void Board::placeShip(int x, int y, int length, bool horizontal) noexcept {
    const int dx = horizontal ? 1 : 0;
    const int dy = horizontal ? 0 : 1;

    for (int i = 0; i < length; ++i) {
        const int cx = x + dx * i;
        const int cy = y + dy * i;

        cells_[cy][cx] = CellState::Ship;
        ++shipsCellsTotal_;
    }
}

// ------------------------------------------------------------
//  Случайная расстановка флота
// ------------------------------------------------------------
void Board::randomPlaceFleet(std::mt19937& rng) {
    reset();

    std::uniform_int_distribution<int> distXY(0, BOARD_SIZE - 1);
    std::uniform_int_distribution<int> distDir(0, 1);

    for (int len : SHIP_SIZES) {
        bool placed = false;

        for (int attempts = 0; attempts < 1000 && !placed; ++attempts) {
            const int x = distXY(rng);
            const int y = distXY(rng);
            const bool horizontal = (distDir(rng) == 0);

            if (canPlaceShip(x, y, len, horizontal)) {
                placeShip(x, y, len, horizontal);
                placed = true;
            }
        }

        if (!placed) {
            std::cerr << "Не удалось разместить корабль длины " << len << '\n';
        }
    }
}

// ------------------------------------------------------------
//  Проверка, затоплен ли корабль, к которому относится (x, y)
//  Предполагается, что (x, y) - Hit либо только что Hit.
// ------------------------------------------------------------
bool Board::isShipSunk(int x, int y) const noexcept {
    constexpr std::array<std::pair<int, int>, 4> dirs{
        std::pair{ 1, 0 },
        std::pair{-1, 0 },
        std::pair{ 0, 1 },
        std::pair{ 0,-1 }
    };

    for (auto [dx, dy] : dirs) {
        int nx = x;
        int ny = y;

        while (isInside(nx, ny)) {
            const auto st = cells_[ny][nx];

            if (st == CellState::Ship)
                return false;  // есть живая палуба

            if (st != CellState::Hit)
                break;         // вышли за пределы корабля

            nx += dx;
            ny += dy;
        }
    }
    return true;
}

// ------------------------------------------------------------
//  Пометить корабль как затопленный (все его палубы)
//  Предполагается, что isShipSunk(x, y) == true.
// ------------------------------------------------------------
void Board::markShipSunk(int x, int y) noexcept {
    constexpr std::array<std::pair<int, int>, 4> dirs{
        std::pair{ 1, 0 },
        std::pair{-1, 0 },
        std::pair{ 0, 1 },
        std::pair{ 0,-1 }
    };

    cells_[y][x] = CellState::Sunk;

    for (auto [dx, dy] : dirs) {
        int nx = x + dx;
        int ny = y + dy;

        while (isInside(nx, ny) && cells_[ny][nx] == CellState::Hit) {
            cells_[ny][nx] = CellState::Sunk;
            nx += dx;
            ny += dy;
        }
    }
}

// ------------------------------------------------------------
//  Выстрел по клетке
// ------------------------------------------------------------
ShotResult Board::shoot(int x, int y) noexcept {
    if (!isInside(x, y))
        return ShotResult::Invalid;

    auto& cell = cells_[y][x];

    // Повторный выстрел
    if (isShot(cell))
        return ShotResult::Repeat;

    // Попадание
    if (cell == CellState::Ship) {
        cell = CellState::Hit;
        ++shipsCellsHit_;

        if (isShipSunk(x, y)) {
            markShipSunk(x, y);
            return ShotResult::Sunk;
        }

        return ShotResult::Hit;
    }

    // Промах
    cell = CellState::Miss;
    return ShotResult::Miss;
}

// ------------------------------------------------------------
//  Проверка уничтожения всего флота
// ------------------------------------------------------------
bool Board::allShipsDestroyed() const noexcept {
    return shipsCellsTotal_ > 0 &&
        shipsCellsHit_ == shipsCellsTotal_;
}
