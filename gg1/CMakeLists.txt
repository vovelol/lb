# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(MyWxWidgetsApp)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Находим пакет wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Добавляем путь к заголовочным файлам
# Используем target_include_directories для привязки путей к исполняемому файлу
include_directories(${CMAKE_SOURCE_DIR}/include)

# Добавляем исходные файлы
set(SOURCES
    src/app.cpp
    src/main.cpp
    src/Ship.cpp
    src/ShipManager.cpp
    src/Field.cpp
    src/Game.cpp
    src/settings_dialog.cpp
    src/bitmap_renderer.cpp
    src/ship_drop_target.cpp
)

# Создаем исполняемый файл
add_executable(MyWxWidgetsApp ${SOURCES})

# Привязываем include директории к цели (исполняемому файлу)
target_include_directories(MyWxWidgetsApp PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Линкуем с библиотекой wxWidgets
target_link_libraries(MyWxWidgetsApp ${wxWidgets_LIBRARIES})
