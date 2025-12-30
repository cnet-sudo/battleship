#pragma once

#include <vector>
#include <random>
#include <optional>
#include <array>
#include <SFML/System/Vector2.hpp>

#include "Board.h"
#include "ShotsGrid.h"
#include "AIState.h"
#include "ProbabilityMap.h"
#include "GameConfig.h"

/**
 * @class AIController
 * @brief Логика искусственного интеллекта для игры «Морской бой».
 *
 * Реализует:
 *  - вероятностный поиск кораблей
 *  - добивание после попаданий
 *  - определение и удержание направления корабля
 *  - корректную обработку затопления
 *  - запрет стрельбы по диагонали от попаданий (классические правила)
 */
class AIController {
public:
    explicit AIController(std::mt19937& rng) noexcept;

    /**
     * @brief Выполняет ход ИИ.
     *
     * @param playerBoard  Поле игрока
     * @param shots        Сетка выстрелов ИИ
     * @param playerTurn   Флаг хода игрока
     * @param playerWon    Флаг победы игрока
     * @return true        Если игра завершена
     */
    [[nodiscard]]
    bool takeTurn(
        Board& playerBoard,
        ShotsGrid& shots,
        bool& playerTurn,
        bool& playerWon
    ) noexcept;

private:
    // Внешний генератор случайных чисел
    std::mt19937& rng_;

    // Текущее состояние логики ИИ (попадания, направление, список целей)
    AIState state_;

    // Вероятностная карта для поиска кораблей
    ProbabilityMap prob_;

private:
    /**
     * @brief Помечает все клетки вокруг попадания как недоступные для стрельбы.
     *
     * Использует классические правила: корабли не могут касаться даже по диагонали,
     * поэтому 8 соседних клеток вокруг попадания заведомо пусты.
     */
    void markForbiddenAroundHit(
        int x, int y,
        const Board& board,
        ShotsGrid& shots
    ) noexcept;

    /**
     * @brief Добавляет допустимые соседние клетки вокруг попадания
     *        (только по 4 направлениям: вверх/вниз/влево/вправо).
     *
     * Используется до того, как определено направление корабля.
     */
    void addNeighborsSmart(
        int x, int y,
        const Board& board,
        const ShotsGrid& shots
    ) noexcept;

    /**
     * @brief Обрабатывает попадание по кораблю и обновляет состояние ИИ.
     *
     * Обновляет список попаданий, помечает запрещённые клетки вокруг,
     * при необходимости определяет направление и формирует цели для добивания.
     */
    void handleHit(
        int x, int y,
        Board& board,
        ShotsGrid& shots
    ) noexcept;

    /**
     * @brief Сортирует список попаданий вдоль текущего направления dx/dy.
     *
     * Для горизонтальных кораблей сортирует по x, для вертикальных — по y.
     */
    void sortHits() noexcept;

    /**
     * @brief Сбрасывает направление и пересобирает цели вокруг всех попаданий.
     *
     * Используется, если направление потеряло смысл, но корабль ещё не добит.
     */
    void resetDirectionAndRebuildTargets(
        const Board& board,
        const ShotsGrid& shots
    ) noexcept;
};
