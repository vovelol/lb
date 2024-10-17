#include "Field.h"
#include <vector>
#include <iostream>
#include <wx/log.h> // Для использования wxLogError

Field::Field(int width, int height, ShipManager& shipManager)
        : width_(width), height_(height), shipManager_(shipManager) {
    field_ = std::vector<std::vector<char>>(height_, std::vector<char>(width_, '.'));
}

void Field::clear() {
    for (auto& row : field_) {
        std::fill(row.begin(), row.end(), '.');
    }
}

bool Field::canPlaceShip(int x, int y, int shipSize, bool isVertical) const {
    wxLogMessage("Проверка возможности размещения корабля на координатах (%d, %d) с размером %d и ориентацией %s",
                 x, y, shipSize, isVertical ? "вертикально" : "горизонтально");

    if (isVertical) {
        if (y + shipSize > height_) {
            wxLogMessage("Ошибка: корабль выходит за пределы поля по вертикали на координатах (%d, %d)", x, y);
            return false;
        }
    } else {
        if (x + shipSize > width_) {
            wxLogMessage("Ошибка: корабль выходит за пределы поля по горизонтали на координатах (%d, %d)", x, y);
            return false;
        }
    }

    for (int i = 0; i < shipSize; ++i) {
        int checkX = isVertical ? x : x + i;
        int checkY = isVertical ? y + i : y;

        if (field_[checkY][checkX] != '.') {
            wxLogMessage("Ошибка: клетка занята другим кораблем на (%d, %d)", checkX, checkY);
            return false;
        }
    }

    return true;
}

bool Field::canPlaceShipWithNeighbors(int x, int y, int shipSize, bool isVertical) const {
    wxLogMessage("Проверка canPlaceShipWithNeighbors для корабля на координатах (%d, %d) с размером %d и ориентацией %s",
                 x, y, shipSize, isVertical ? "вертикально" : "горизонтально");

    // Основная проверка на возможность размещения корабля в указанных координатах
    if (!canPlaceShip(x, y, shipSize, isVertical)) {
        wxLogMessage("Основная проверка canPlaceShip не пройдена на координатах (%d, %d)", x, y);
        return false;
    }

    // Проверка соседних клеток для предотвращения размещения кораблей вплотную
    int startX = std::max(0, x - 1);
    int startY = std::max(0, y - 1);
    int endX = isVertical ? std::min(width_ - 1, x + 1) : std::min(width_ - 1, x + shipSize);
    int endY = isVertical ? std::min(height_ - 1, y + shipSize) : std::min(height_ - 1, y + 1);

    wxLogMessage("Проверка соседних клеток от (%d, %d) до (%d, %d)", startX, startY, endX, endY);

    // Проход по клеткам для проверки наличия соседних кораблей
    for (int row = startY; row <= endY; ++row) {
        for (int col = startX; col <= endX; ++col) {
            // Пропускаем клетки, которые принадлежат самому кораблю
            if (isVertical && col == x && row >= y && row < y + shipSize) {
                continue;
            }
            if (!isVertical && row == y && col >= x && col < x + shipSize) {
                continue;
            }

            if (field_[row][col] != '.') {
                wxLogMessage("Невозможно разместить корабль: соседняя клетка занята на (%d, %d)", col, row);
                return false;
            }
        }
    }

    return true;
}

void Field::placeShip(int x, int y, int shipSize, bool isVertical) {
    wxLogMessage("Размещение корабля на координатах (%d, %d) с размером %d и ориентацией %s",
                 x, y, shipSize, isVertical ? "вертикально" : "горизонтально");

    if (isVertical) {
        for (int i = 0; i < shipSize; ++i) {
            field_[y + i][x] = 'S';
        }
    } else {
        for (int i = 0; i < shipSize; ++i) {
            field_[y][x + i] = 'S';
        }
    }
}

bool Field::isCellOccupied(int x, int y) const {
    return field_[y][x] != '.';
}

void Field::markCellAsHit(int x, int y) {
    if (field_[y][x] == 'S') {
        field_[y][x] = 'H';
    }
}

void Field::markCellAsMiss(int x, int y) {
    if (field_[y][x] == '.') {
        field_[y][x] = 'M';
    }
}

bool Field::attackCell(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        wxLogMessage("Ошибка: координаты (%d, %d) выходят за пределы поля", x, y);
        return false;
    }

    if (field_[y][x] == 'S') {
        field_[y][x] = 'H';
        markCellAsHit(x, y);
        wxLogMessage("Попадание по кораблю на координатах (%d, %d)", x, y);
        return true;
    } else if (field_[y][x] == '.') {
        field_[y][x] = 'M';
        markCellAsMiss(x, y);
        wxLogMessage("Промах по координатам (%d, %d)", x, y);
        return false;
    }

    wxLogMessage("Попадание в ранее атакованную клетку на координатах (%d, %d)", x, y);
    return false;
}
