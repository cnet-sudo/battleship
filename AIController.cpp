#include "AIController.h"
#include <algorithm>
#include <iostream>


AIController::AIController(std::mt19937& rng) noexcept
    : rng_(rng)
{
}


//  Помечаем все клетки вокруг затопленного корабля как недоступные

void AIController::markForbiddenAroundHit(
    int x, int y,
    const Board& board,
    ShotsGrid& shots
) noexcept
{
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {

            if (dx == 0 && dy == 0)
                continue;

            int nx = x + dx;
            int ny = y + dy;

            if (!board.isInside(nx, ny))
                continue;

            shots[ny][nx] = true;
        }
    }
}


//  Добавление соседних клеток (только 4 направления)
void AIController::addNeighborsSmart(
    int x, int y,
    const Board& board,
    const ShotsGrid& shots
) noexcept
{
    if (state_.hasDirection)
        return;

    static const std::array<sf::Vector2i, 4> dirs{
        sf::Vector2i{ 1, 0 },
        sf::Vector2i{-1, 0 },
        sf::Vector2i{ 0, 1 },
        sf::Vector2i{ 0,-1 }
    };

    for (auto d : dirs) {
        int nx = x + d.x;
        int ny = y + d.y;

        if (!board.isInside(nx, ny))
            continue;

        if (shots[ny][nx])
            continue;

        auto st = board.cells()[ny][nx];
        if (st == CellState::Miss || st == CellState::Sunk)
            continue;

        state_.targets.emplace_back(nx, ny);
    }
}


//  Сортировка попаданий вдоль направления

void AIController::sortHits() noexcept
{
    if (!state_.hasDirection || state_.hitCells.size() < 2)
        return;

    if (state_.dx != 0) {
        std::ranges::sort(state_.hitCells, {}, &sf::Vector2i::x);
    }
    else {
        std::ranges::sort(state_.hitCells, {}, &sf::Vector2i::y);
    }
}

//  Сброс направления и пересборка целей

void AIController::resetDirectionAndRebuildTargets(
    const Board& board,
    const ShotsGrid& shots
) noexcept
{
    state_.hasDirection = false;
    state_.dx = state_.dy = 0;
    state_.targets.clear();

    for (const auto& h : state_.hitCells)
        addNeighborsSmart(h.x, h.y, board, shots);
}

//  Обработка попадания
void AIController::handleHit(
    int x, int y,
    Board& board,
    ShotsGrid& shots
) noexcept
{
    state_.addHit({ x, y });

    // Если корабль затоплен — закрываем диагонали вокруг всех палуб
    if (board.cells()[y][x] == CellState::Sunk) {

        for (const auto& h : state_.hitCells)
            markForbiddenAroundHit(h.x, h.y, board, shots);

        markForbiddenAroundHit(x, y, board, shots);

        state_.resetShipTracking();
        return;
    }

    // Первый хит — просто добавляем соседей
    if (state_.hitCells.size() == 1) {
        addNeighborsSmart(x, y, board, shots);
        return;
    }

    // Второй хит — определяем направление
    if (state_.hitCells.size() == 2 && !state_.hasDirection) {

        std::ranges::sort(state_.hitCells, [](const auto& a, const auto& b) {
            if (a.x == b.x) return a.y < b.y;
            return a.x < b.x;
            });

        const auto& a = state_.hitCells[0];
        const auto& b = state_.hitCells[1];

        // ЧЁТКОЕ ПРАВИЛО:
        // если совпадает X → вертикаль
        // если совпадает Y → горизонталь

        if (a.x == b.x && a.y != b.y) {
            state_.dx = 0;
            state_.dy = (b.y > a.y ? 1 : -1);
            state_.hasDirection = true;
        }
        else if (a.y == b.y && a.x != b.x) {
            state_.dx = (b.x > a.x ? 1 : -1);
            state_.dy = 0;
            state_.hasDirection = true;
        }

        sortHits();
    }

    // Добивание вдоль направления
    if (state_.hasDirection) {
        sortHits();

        const auto& first = state_.hitCells.front();
        const auto& last = state_.hitCells.back();

        auto canShoot = [&](int cx, int cy) noexcept {
            if (!board.isInside(cx, cy)) return false;
            if (shots[cy][cx]) return false;

            auto st = board.cells()[cy][cx];
            return st != CellState::Miss && st != CellState::Sunk;
            };

        // Попытка выстрелить вперёд
        int nx = last.x + state_.dx;
        int ny = last.y + state_.dy;

        if (canShoot(nx, ny)) {
            state_.targets.emplace_back(nx, ny);
            return;
        }

        // Попытка выстрелить назад
        nx = first.x - state_.dx;
        ny = first.y - state_.dy;

        if (canShoot(nx, ny)) {
            state_.targets.emplace_back(nx, ny);
            return;
        }

        // Оба направления закрыты — корабль добит
        state_.resetShipTracking();
    }
}

//  Основной ход ИИ
bool AIController::takeTurn(
    Board& playerBoard,
    ShotsGrid& shots,
    bool& playerTurn,
    bool& playerWon
) noexcept
{
    std::optional<sf::Vector2i> chosen;

    // Удаляем недействительные цели
    std::erase_if(state_.targets, [&](const sf::Vector2i& t) {
        return !playerBoard.isInside(t.x, t.y) || shots[t.y][t.x];
        });

    // Если есть цели — берём последнюю
    if (!state_.targets.empty()) {
        chosen = state_.targets.back();
        state_.targets.pop_back();
    }

    // Иначе — вероятностная карта
    if (!chosen) {
        prob_.compute(playerBoard, shots);

        int bestScore = -1;
        std::vector<sf::Vector2i> best;

        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (shots[y][x]) continue;

                int score = prob_.map[y][x];

                if (score > bestScore) {
                    bestScore = score;
                    best.clear();
                    best.emplace_back(x, y);
                }
                else if (score == bestScore) {
                    best.emplace_back(x, y);
                }
            }
        }

        std::uniform_int_distribution<size_t> dist(0, best.size() - 1);
        chosen = best[dist(rng_)];
    }

    auto [tx, ty] = *chosen;
    shots[ty][tx] = true;

    ShotResult result = playerBoard.shoot(tx, ty);

    switch (result) {
    case ShotResult::Hit:
        handleHit(tx, ty, playerBoard, shots);
        playerTurn = false;
        break;

    case ShotResult::Sunk:
        handleHit(tx, ty, playerBoard, shots);
        if (playerBoard.allShipsDestroyed()) {
            playerWon = false;
            return true;
        }
        playerTurn = false;
        break;

    case ShotResult::Miss:
        // если мы охотились — не теряем корабль
        if (!state_.hitCells.empty()) {
            resetDirectionAndRebuildTargets(playerBoard, shots);
        }
        playerTurn = true;
        break;

    default:
        break;
    }

    return false;
}
