#include "Field.h"
#include <vector>
#include <iostream>

Field::Field(int width, int height, ShipManager& shipManager)
    : width_(width), height_(height), shipManager_(shipManager) {
    field_ = std::vector<std::vector<char>>(height_, std::vector<char>(width_, '.'));
}

bool Field::canPlaceShip(int x, int y, int shipSize, bool isVertical) const {
    if (isVertical) {
        if (y + shipSize > height_) {
            return false;
        }
        for (int i = 0; i < shipSize; ++i) {
            if (field_[y + i][x] != '.') {
                return false;
            }
        }
    } else {
        if (x + shipSize > width_) {
            return false;
        }
        for (int i = 0; i < shipSize; ++i) {
            if (field_[y][x + i] != '.') {
                return false;
            }
        }
    }
    return true;
}

bool Field::canPlaceShipWithNeighbors(int x, int y, int shipSize, bool isVertical) const {
    if (!canPlaceShip(x, y, shipSize, isVertical)) {
        return false;
    }

    // Проверяем соседние клетки вокруг предполагаемого места размещения
    for (int i = -1; i <= shipSize; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int checkX = x + (isVertical ? j : i);
            int checkY = y + (isVertical ? i : j);

            if (checkX >= 0 && checkX < width_ && checkY >= 0 && checkY < height_) {
                if (field_[checkY][checkX] != '.') {
                    return false;
                }
            }
        }
    }
    return true;
}

void Field::placeShip(int x, int y, int shipSize, bool isVertical) {
    if (isVertical) {
        for (int i = 0; i < shipSize; ++i) {
            field_[y + i][x] = 'S';  // 'S' для обозначения корабля
        }
    } else {
        for (int i = 0; i < shipSize; ++i) {
            field_[y][x + i] = 'S';  // 'S' для обозначения корабля
        }
    }
}

bool Field::isCellOccupied(int x, int y) const {
    return field_[y][x] != '.';
}

void Field::markCellAsHit(int x, int y) {
    if (field_[y][x] == 'S') {
        field_[y][x] = 'H';  // Обозначаем попадание
    }
}

void Field::markCellAsMiss(int x, int y) {
    if (field_[y][x] == '.') {
        field_[y][x] = 'M';  // Обозначаем промах
    }
}

bool Field::attackCell(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return false;  // Координаты за пределами поля
    }

    if (field_[y][x] == 'S') {
        // Попадание по кораблю
        field_[y][x] = 'H';
        markCellAsHit(x, y);
        return true;
    } else if (field_[y][x] == '.') {
        // Промах
        field_[y][x] = 'M';
        markCellAsMiss(x, y);
        return false;
    }

    // Если уже стреляли в эту клетку (то есть она уже помечена 'H' или 'M')
    return false;
}
