#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <array>
#include <iostream>
#include <ctime>        // для std::time

#include "Board.h"
#include "AIController.h"
#include "Renderer.h"
#include "GameConfig.h"

/**
 * Game
 *
 * Управляет жизненным циклом игры:
 * инициализация, обработка событий, логика и отрисовка.
 */
class Game {
public:
    Game();
    void run();

private:
    // Окно приложения
    sf::RenderWindow window_;

    // Шрифт и статусная строка
    sf::Font font_;
    sf::Text statusText_;

    // Основные игровые объекты
    std::mt19937 rng_;
    Board playerBoard_;
    Board aiBoard_;
    AIController aiController_;
    Renderer renderer_;

    // Сетка выстрелов ИИ по полю игрока
    std::array<std::array<bool, BOARD_SIZE>, BOARD_SIZE> aiShots_{};

    // Состояние игры
    bool playerTurn_ = true;
    bool gameOver_ = false;
    bool playerWon_ = false;

    // Позиционирование игровых полей
    int playerBoardX_ = 0;
    int aiBoardX_ = 0;
    int boardsY_ = 40;

    // Ограничение времени хода (секунд)
    sf::Clock turnClock_;
    float turnTimeLimit_ = 5.0f;

    // Внутренняя логика
    void handleEvents();
    void handlePlayerClick(int mouseX, int mouseY);
    void updateStatusText();

    // Курсоры мыши
    sf::Cursor cursorArrow_;
    sf::Cursor cursorCrosshair_;
};
