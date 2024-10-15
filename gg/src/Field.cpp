#include "Field.h"

Field::Field(int w, int h, ShipManager& manager)
    : width(w), height(h), shipManager(manager) {
    grid.resize(height, std::vector<CellState>(width, EMPTY));
}

bool Field::isCellOccupied(int x, int y) const {
    return grid[y][x] == SHIP;
}

void Field::markCellAsHit(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height && grid[y][x] == SHIP) {
        grid[y][x] = HIT;
    }
}

void Field::markCellAsMiss(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = MISS;
    }
}

void Field::placeShip(int x, int y, int length, bool isVertical) {
    for (int i = 0; i < length; ++i) {
        if (isVertical) {
            grid[y + i][x] = SHIP;
        } else {
            grid[y][x + i] = SHIP;
        }
    }
}

bool Field::canPlaceShip(int x, int y, int length, bool isVertical) const {
    for (int i = 0; i < length; ++i) {
        if (isVertical) {
            if (y + i >= height || grid[y + i][x] != EMPTY) {
                return false;  // Корабль выходит за границы или пересекается с другим кораблем
            }
        } else {
            if (x + i >= width || grid[y][x + i] != EMPTY) {
                return false;  // Корабль выходит за границы или пересекается с другим кораблем
            }
        }
    }
    return true;
}
