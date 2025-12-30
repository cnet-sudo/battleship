#pragma once
#include <SFML/Graphics.hpp>

// Пространство имён Style содержит ВСЕ визуальные настройки игры.
// Это позволяет централизованно управлять оформлением,
// не меняя код рендера или игровой логики.
namespace Style {

    // --- ФОН ИГРОВОГО ОКНА ---
    // Основной цвет фона, на котором рисуются оба поля.
    inline const sf::Color Background = sf::Color(20, 30, 40);

    // --- ЦВЕТА КЛЕТОК ПОЛЯ ---
    // Цвет пустой клетки (ничего не происходило).
    inline const sf::Color CellDefault = sf::Color(30, 60, 90);

    // Цвет клетки с кораблём (виден только на поле игрока).
    inline const sf::Color CellShip = sf::Color(70, 120, 170);

    // Цвет клетки, куда был сделан промах.
    inline const sf::Color CellMiss = sf::Color(80, 80, 80);

    // Цвет клетки, куда попали, но корабль ещё не затоплен.
    inline const sf::Color CellHit = sf::Color(200, 50, 50);

    // Цвет клетки затопленного корабля.
    inline const sf::Color CellSunk = sf::Color(150, 0, 0);

    // Цвет линий крестика, которым помечается затопленный корабль.
    inline const sf::Color SunkLine = sf::Color(255, 255, 255);



    // --- НАСТРОЙКИ ТЕКСТА ---
    // Размер шрифта для текста статуса (чей ход, победа и т.п.).
    inline constexpr unsigned FontSize = 24;

    // Цвет текста.
    inline const sf::Color TextColor = sf::Color::White;
}
