#pragma once

#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameConfig.h"
#include "StyleConfig.h"

class Renderer {
public:
    explicit Renderer(sf::RenderWindow& window) noexcept;

    // Отрисовка двух полей и текста статуса
    void draw(
        const Board& playerBoard,
        const Board& aiBoard,
        const sf::Text& statusText,
        int playerBoardX,
        int aiBoardX,
        int boardsY
    ) const noexcept;

private:
    sf::RenderWindow& window_;

    // Отрисовка одного поля
    void drawBoard(
        const Board& board,
        int offsetX,
        int offsetY,
        bool showShips
    ) const noexcept;
};
