#pragma once

#include <array>
#include "Board.h"
#include "ShotsGrid.h"
#include "GameConfig.h"

/**
 * ProbabilityMap
 *
 * Строит карту вероятностей для выбора следующего выстрела ИИ.
 * Учитывает:
 *  - промахи (Miss)
 *  - попадания (Hit)
 *  - затопленные корабли (Sunk)
 *  - размеры оставшихся кораблей
 */
class ProbabilityMap {
public:
    // Карта вероятностей
    std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> map{};

    /**
     * Пересчитывает карту вероятностей.
     */
    void compute(
        const Board& board,
        const ShotsGrid& shots
    ) noexcept;

private:
    /**
     * Проверяет, можно ли разместить корабль длины length
     * начиная с (x, y) в направлении horizontal.
     */
    bool canPlace(
        const Board& board,
        const ShotsGrid& shots,
        int x, int y,
        int length,
        bool horizontal
    ) const noexcept;
};
