#pragma once

#include <vector>
#include <algorithm>
#include <SFML/System/Vector2.hpp>

// Состояние ИИ при охоте за кораблём
struct AIState {
    // Кандидаты для следующего выстрела
    std::vector<sf::Vector2i> targets;

    // Клетки, в которые уже было попадание по текущему кораблю
    std::vector<sf::Vector2i> hitCells;

    // Определено ли направление корабля
    bool hasDirection = false;

    // Направление (dx, dy): горизонталь или вертикаль
    int dx = 0;
    int dy = 0;

    // Добавление попадания с защитой от дубликатов
    void addHit(const sf::Vector2i& p) noexcept {
        if (std::find(hitCells.begin(), hitCells.end(), p) == hitCells.end())
            hitCells.push_back(p);
    }

    // Сброс состояния охоты
    void resetShipTracking() noexcept {
        targets.clear();
        hitCells.clear();
        hasDirection = false;
        dx = 0;
        dy = 0;
    }

    // Проверка: есть ли активная охота
    [[nodiscard]]
    bool isHunting() const noexcept {
        return !hitCells.empty();
    }

    // Проверка: есть ли цели
    [[nodiscard]]
    bool hasTargets() const noexcept {
        return !targets.empty();
    }
};
