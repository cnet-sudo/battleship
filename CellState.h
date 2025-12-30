#pragma once

#include <string_view>

enum class CellState : unsigned char {
    Empty,
    Ship,
    Miss,
    Hit,
    Sunk
};


[[nodiscard]]
constexpr std::string_view toString(CellState state) noexcept {
    switch (state) {
    case CellState::Empty: return "Empty";
    case CellState::Ship:  return "Ship";
    case CellState::Miss:  return "Miss";
    case CellState::Hit:   return "Hit";
    case CellState::Sunk:  return "Sunk";
    default:               return "Unknown";
    }
}

// Проверки состояния клетки
[[nodiscard]]
constexpr bool isShot(CellState s) noexcept {
    return s == CellState::Miss ||
        s == CellState::Hit ||
        s == CellState::Sunk;
}

[[nodiscard]]
constexpr bool isShip(CellState s) noexcept {
    return s == CellState::Ship ||
        s == CellState::Hit ||
        s == CellState::Sunk;
}
