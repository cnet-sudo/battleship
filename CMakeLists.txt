cmake_minimum_required(VERSION 3.20)
project(BattleshipGame LANGUAGES CXX)

# Стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Сканируем все .cpp файлы в корне
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")

# Создаём исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключаем SFML 2.6.0
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics)

# Копируем ВСЕ ресурсы из корня (кроме CMake и build)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/arial.ttf"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
