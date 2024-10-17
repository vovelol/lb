# File: CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(MyWxWidgetsApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Используем wx-config для получения флагов компиляции и линковки
find_program(wxWidgets_CONFIG_EXECUTABLE NAMES wx-config)

if (wxWidgets_CONFIG_EXECUTABLE)
    message(STATUS "Found wx-config: ${wxWidgets_CONFIG_EXECUTABLE}")

    # Запрашиваем wx-config флаги компиляции и линковки
    execute_process(COMMAND ${wxWidgets_CONFIG_EXECUTABLE} --cxxflags
            OUTPUT_VARIABLE wxWidgets_CXX_FLAGS
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${wxWidgets_CONFIG_EXECUTABLE} --libs
            OUTPUT_VARIABLE wxWidgets_LINK_FLAGS
            OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Устанавливаем флаги компилятора и линковщика
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${wxWidgets_LINK_FLAGS}")

else()
    message(FATAL_ERROR "wx-config не найден. Убедитесь, что wxWidgets установлен, и wx-config доступен.")
endif()

# Включение директории с заголовочными файлами
include_directories(${CMAKE_SOURCE_DIR}/include)

# Определение исходных файлов
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

set(HEADERS
        include/common.h
        include/app.h
        include/Game.h
        include/Field.h
        include/Ship.h
        include/ShipManager.h
        include/settings_dialog.h
        include/bitmap_renderer.h
        include/ship_drop_target.h
        include/main.h
)

# Создание исполняемого файла
add_executable(MyWxWidgetsApp ${SOURCES} ${HEADERS})

# Добавляем полученные флаги линковки для wxWidgets
target_link_libraries(MyWxWidgetsApp ${wxWidgets_LINK_FLAGS})
