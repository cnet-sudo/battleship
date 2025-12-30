#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& window) noexcept
    : window_(window) {
    // Сохраняем ссылку на окно для дальнейшего рендера
}

void Renderer::drawBoard(const Board& board,
    int offsetX,
    int offsetY,
    bool showShips) const noexcept
{
    // Базовая форма клетки
    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    const auto& cells = board.cells();

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {

            // Позиция клетки на экране
            float px = float(offsetX + x * CELL_SIZE + 1);
            float py = float(offsetY + y * CELL_SIZE + 1);
            cellShape.setPosition(px, py);

            // Определяем цвет клетки по её состоянию
            CellState st = cells[y][x];
            sf::Color color = Style::CellDefault;

            if (showShips && st == CellState::Ship)
                color = Style::CellShip;
            if (st == CellState::Miss)
                color = Style::CellMiss;
            if (st == CellState::Hit)
                color = Style::CellHit;
            if (st == CellState::Sunk)
                color = Style::CellSunk;

            cellShape.setFillColor(color);
            window_.draw(cellShape);

            // Рисуем крестик на потопленном корабле
            if (st == CellState::Sunk) {
                sf::Vertex line1[] = {
                    sf::Vertex(sf::Vector2f(px, py), Style::SunkLine),
                    sf::Vertex(sf::Vector2f(px + CELL_SIZE - 2, py + CELL_SIZE - 2), Style::SunkLine)
                };

                sf::Vertex line2[] = {
                    sf::Vertex(sf::Vector2f(px + CELL_SIZE - 2, py), Style::SunkLine),
                    sf::Vertex(sf::Vector2f(px, py + CELL_SIZE - 2), Style::SunkLine)
                };

                window_.draw(line1, 2, sf::Lines);
                window_.draw(line2, 2, sf::Lines);
            }
        }
    }
}

void Renderer::draw(const Board& playerBoard,
    const Board& aiBoard,
    const sf::Text& statusText,
    int playerBoardX,
    int aiBoardX,
    int boardsY) const noexcept
{
    // Очищаем экран
    window_.clear(Style::Background);

    // Рисуем оба поля
    drawBoard(playerBoard, playerBoardX, boardsY, true);   // показываем корабли игрока
    drawBoard(aiBoard, aiBoardX, boardsY, false);          // скрываем корабли AI

    // Текст статуса (ход, победа и т.п.)
    window_.draw(statusText);

    // Выводим кадр
    window_.display();
}
