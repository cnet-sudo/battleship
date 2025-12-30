cmake_minimum_required(VERSION 3.20)
project(BattleshipGame LANGUAGES CXX)

# Стандарт C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явно перечисляем исходники 
set(SOURCES
    AIController.cpp
    Board.cpp
    Game.cpp
    ProbabilityMap.cpp
    Renderer.cpp
    battleship.cpp
)

# Создаём исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключаем SFML 2.6.0
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics)

# Копируем ресурсы (все файлы в корне)
file(GLOB RESOURCE_FILES
    "${CMAKE_SOURCE_DIR}/*.png"
    "${CMAKE_SOURCE_DIR}/*.jpg"
    "${CMAKE_SOURCE_DIR}/*.ttf"
    "${CMAKE_SOURCE_DIR}/*.wav"
    "${CMAKE_SOURCE_DIR}/*.json"
)

foreach(RES ${RESOURCE_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${RES}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach()

