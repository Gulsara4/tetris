cmake_minimum_required(VERSION 3.16)

project(snake_gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Список исходников игры + GUI
set(PROJECT_SOURCES
    main.cpp
    gamewindow.cpp
    ../../../brick_game/snake/apple.cpp
    ../../../brick_game/snake/functions_of_fields.cpp
    ../../../brick_game/snake/snake.cpp

)

set(PROJECT_HEADERS
    gamewindow.h
    ../../../brick_game/snake/game.h
    ../../../brick_game/snake/classes_snake_apple.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(snake_gui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
    )
else()
    add_executable(snake_gui ${PROJECT_SOURCES} ${PROJECT_HEADERS})
endif()

target_link_libraries(snake_gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
