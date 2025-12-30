cmake_minimum_required(VERSION 3.20)
project(BattleshipGame LANGUAGES CXX)

# Стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к исходникам, заголовкам и ресурсам
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

# Сканируем все .cpp файлы
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

# Создаём исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключаем заголовки
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

# Подключаем SFML 2.6.0
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics)

# Копируем ресурсы (всё из assets/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_DIR}"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
