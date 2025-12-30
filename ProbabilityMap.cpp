#include "ProbabilityMap.h"

// ------------------------------------------------------------
// Проверка возможности разместить корабль
// ------------------------------------------------------------
bool ProbabilityMap::canPlace(
    const Board& board,
    const ShotsGrid& shots,
    int x, int y,
    int length,
    bool horizontal
) const noexcept
{
    const int dx = horizontal ? 1 : 0;
    const int dy = horizontal ? 0 : 1;

    for (int i = 0; i < length; ++i) {
        const int cx = x + dx * i;
        const int cy = y + dy * i;

        if (!board.isInside(cx, cy))
            return false;

        const auto cell = board.cells()[cy][cx];

        // Здесь корабля точно нет
        if (cell == CellState::Miss)
            return false;

        // Здесь уже лежит добитый корабль — новый тут быть не может
        if (cell == CellState::Sunk)
            return false;

        // Ship НЕ запрещаем!
        // ИИ не знает, где корабли игрока.
        // Hit допустим — это значит, что корабль проходит через эту клетку.
    }

    return true;
}

// ------------------------------------------------------------
// Пересчёт карты вероятностей
// ------------------------------------------------------------
void ProbabilityMap::compute(
    const Board& board,
    const ShotsGrid& shots
) noexcept
{
    // Обнуляем карту
    for (auto& row : map)
        row.fill(0);

    // Перебираем все размеры кораблей
    for (int len : SHIP_SIZES) {

        for (int y = 0; y < static_cast<int>(BOARD_SIZE); ++y) {
            for (int x = 0; x < static_cast<int>(BOARD_SIZE); ++x) {

                // Горизонтально
                if (canPlace(board, shots, x, y, len, true)) {
                    for (int i = 0; i < len; ++i)
                        map[y][x + i]++;
                }

                // Вертикально
                if (canPlace(board, shots, x, y, len, false)) {
                    for (int i = 0; i < len; ++i)
                        map[y + i][x]++;
                }
            }
        }
    }
}
