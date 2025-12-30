#include "Game.h"

Game::Game()
    : window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), L"Морской бой", sf::Style::Titlebar | sf::Style::Close),
    rng_(static_cast<unsigned>(std::time(nullptr))),
    aiController_(rng_),
    renderer_(window_)
{
    window_.setFramerateLimit(60);

   
    // Шрифт
    
    if (!font_.loadFromFile("arial.ttf")) {
        std::cerr << "Не удалось загрузить arial.ttf\n";
    }

    statusText_.setFont(font_);
    statusText_.setCharacterSize(24);
    statusText_.setFillColor(sf::Color::White);
    //Курсор 
    cursorArrow_.loadFromSystem(sf::Cursor::Arrow);
    cursorCrosshair_.loadFromSystem(sf::Cursor::Cross);

    
    // Центрирование полей
    
    int boardPixelSize = BOARD_SIZE * CELL_SIZE;
    int gap = 80;

    int totalWidth = boardPixelSize * 2 + gap;
    int startX = (WINDOW_WIDTH - totalWidth) / 2;

    playerBoardX_ = startX;
    aiBoardX_ = startX + boardPixelSize + gap;
    boardsY_ = 40;

   
    // Расстановка кораблей
    
    playerBoard_.randomPlaceFleet(rng_);
    aiBoard_.randomPlaceFleet(rng_);

    
    // Инициализация массива выстрелов ИИ
    
    for (int y = 0; y < BOARD_SIZE; ++y)
        for (int x = 0; x < BOARD_SIZE; ++x)
            aiShots_[y][x] = false;

    updateStatusText();
}

void Game::updateStatusText() {
    float remaining = turnTimeLimit_ - turnClock_.getElapsedTime().asSeconds();
    if (remaining < 0) remaining = 0;

    if (gameOver_) {
        statusText_.setString(playerWon_
            ? L"Вы победили! ЛКМ — выйти."
            : L"Вы проиграли! ЛКМ — выйти.");
    }
    else {
        if (playerTurn_)
            statusText_.setString(L"Ваш ход");
        else
            statusText_.setString(L"Ход компьютера (" + std::to_wstring((int)remaining) + L" сек)");
    }

    statusText_.setPosition(200.f, WINDOW_HEIGHT - 100.f);
}

void Game::handlePlayerClick(int mouseX, int mouseY) {
    if (gameOver_) {
        window_.close();
        return;
    }

    if (!playerTurn_)
        return;

    int boardPixelSize = BOARD_SIZE * CELL_SIZE;

    // Проверяем попадание по правому полю (поле ИИ)
    if (mouseX >= aiBoardX_ && mouseX < aiBoardX_ + boardPixelSize &&
        mouseY >= boardsY_ && mouseY < boardsY_ + boardPixelSize)
    {
        int cellX = (mouseX - aiBoardX_) / CELL_SIZE;
        int cellY = (mouseY - boardsY_) / CELL_SIZE;

        ShotResult result = aiBoard_.shoot(cellX, cellY);

        switch (result) {

        case ShotResult::Invalid:
            // Клик вне поля — игнорируем
            return;

        case ShotResult::Repeat:
            // Уже стреляли — не передаём ход
            return;

        case ShotResult::Miss:
            playerTurn_ = false;
            turnClock_.restart();
            break;

        case ShotResult::Hit:
            // Игрок стреляет снова
            break;

        case ShotResult::Sunk:
            if (aiBoard_.allShipsDestroyed()) {
                gameOver_ = true;
                playerWon_ = true;
            }
            // Игрок стреляет снова
            break;
        }

        updateStatusText();
    }
}


void Game::handleEvents() {
    sf::Event event{};
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window_.close();

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
            handlePlayerClick(mousePos.x, mousePos.y);
        }
    }
}

void Game::run() {
    while (window_.isOpen()) {
        handleEvents();

        
        // Ход компьютера по таймеру
      
        if (!gameOver_ && !playerTurn_) {
            float remaining = turnTimeLimit_ - turnClock_.getElapsedTime().asSeconds();

            if (remaining <= 0) {
                bool ended = aiController_.takeTurn(playerBoard_, aiShots_, playerTurn_, playerWon_);
                if (ended) {
                    gameOver_ = true;
                }
                turnClock_.restart();
            }

            updateStatusText();
        }
        //Курсор
        // Проверяем позицию мыши
        sf::Vector2i mp = sf::Mouse::getPosition(window_);

        int boardPixelSize = BOARD_SIZE * CELL_SIZE;

        // Если мышь над полем противника — ставим прицел
        if (mp.x >= aiBoardX_ && mp.x < aiBoardX_ + boardPixelSize &&
            mp.y >= boardsY_ && mp.y < boardsY_ + boardPixelSize &&
            playerTurn_ && !gameOver_)
        {
            window_.setMouseCursor(cursorCrosshair_);
        }
        else {
            window_.setMouseCursor(cursorArrow_);
        }

        
        // Рендер
       
        renderer_.draw(
            playerBoard_,
            aiBoard_,
            statusText_,
            playerBoardX_,
            aiBoardX_,
            boardsY_
        );
    }
}
