#pragma once

#include <array>
#include "GameConfig.h"

/*
 ShotsGrid.h
 Сетка выстрелов ИИ или игрока.

 Значение true означает, что по клетке уже стреляли.
 Значение false — клетка ещё не проверялась.
 */
using ShotsGrid = std::array<std::array<bool, BOARD_SIZE>, BOARD_SIZE>;
