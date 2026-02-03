cmake_minimum_required(VERSION 3.16)

project(desktop_tetris VERSION 0.1 LANGUAGES CXX C)
add_definitions(-DNTEST)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Пути к исходникам
set(TETRIS_C_SOURCES
    ../../../brick_game/tetris/figure.c
    ../../../brick_game/tetris/field_func.c
    ../../../brick_game/tetris/states.c
)

set(TETRIS_HEADERS
    ../../../brick_game/tetris/s21_tetris.h
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Собираем GUI исполняемый файл
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(desktop_tetris
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${TETRIS_HEADERS}  # добавляем только заголовки
        ${TETRIS_C_SOURCES} # C-файлы
    )
else()
    add_executable(desktop_tetris
        ${PROJECT_SOURCES}
        ${TETRIS_HEADERS}
        ${TETRIS_C_SOURCES}
    )
endif()

# Линковка с Qt Widgets
target_link_libraries(desktop_tetris PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Опция для C-файлов: явно указать, что это C
set_source_files_properties(${TETRIS_C_SOURCES} PROPERTIES LANGUAGE C)

# Свойства для Windows / MacOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.desktop_tetris)
endif()

set_target_properties(desktop_tetris PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Установка
include(GNUInstallDirs)
install(TARGETS desktop_tetris
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Финализация Qt
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(desktop_tetris)
endif()
