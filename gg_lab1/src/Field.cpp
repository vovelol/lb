// File: src/Field.cpp
#include "Field.h"
#include <wx/log.h> // Для использования wxLogMessage

Field::Field(int width, int height, ShipManager& shipManager)
        : width_(width), height_(height), shipManager_(shipManager) {
    field_ = std::vector<std::vector<CellStatus>>(height_, std::vector<CellStatus>(width_, CellStatus::Empty));
}

void Field::clear() {
    for (auto& row : field_) {
        std::fill(row.begin(), row.end(), CellStatus::Empty);
    }
}

bool Field::canPlaceShip(int x, int y, int shipSize, Orientation orientation) const {
    wxLogMessage("Проверка возможности размещения корабля на координатах (%d, %d) с размером %d и ориентацией %s",
                 x, y, shipSize, (orientation == Orientation::Vertical) ? "вертикально" : "горизонтально");

    if (orientation == Orientation::Vertical) {
        if (y + shipSize > height_) {
            wxLogMessage("Ошибка: корабль выходит за пределы поля по вертикали на координатах (%d, %d)", x, y);
            return false;
        }
    } else { // Horizontal
        if (x + shipSize > width_) {
            wxLogMessage("Ошибка: корабль выходит за пределы поля по горизонтали на координатах (%d, %d)", x, y);
            return false;
        }
    }

    for (int i = 0; i < shipSize; ++i) {
        int checkX = (orientation == Orientation::Vertical) ? x : x + i;
        int checkY = (orientation == Orientation::Vertical) ? y + i : y;

        if (field_[checkY][checkX] != CellStatus::Empty) {
            wxLogMessage("Ошибка: клетка занята другим кораблем на (%d, %d)", checkX, checkY);
            return false;
        }
    }

    return true;
}

bool Field::canPlaceShipWithNeighbors(int x, int y, int shipSize, Orientation orientation) const {
    wxLogMessage("Проверка canPlaceShipWithNeighbors для корабля на координатах (%d, %d) с размером %d и ориентацией %s",
                 x, y, shipSize, (orientation == Orientation::Vertical) ? "вертикально" : "горизонтально");

    // Основная проверка на возможность размещения корабля в указанных координатах
    if (!canPlaceShip(x, y, shipSize, orientation)) {
        wxLogMessage("Основная проверка canPlaceShip не пройдена на координатах (%d, %d)", x, y);
        return false;
    }

    // Проверка соседних клеток для предотвращения размещения кораблей вплотную
    int startX = std::max(0, x - 1);
    int startY = std::max(0, y - 1);
    int endX = (orientation == Orientation::Vertical) ? std::min(width_ - 1, x + 1) : std::min(width_ - 1, x + shipSize);
    int endY = (orientation == Orientation::Vertical) ? std::min(height_ - 1, y + shipSize) : std::min(height_ - 1, y + 1);

    wxLogMessage("Проверка соседних клеток от (%d, %d) до (%d, %d)", startX, startY, endX, endY);

    // Проход по клеткам для проверки наличия соседних кораблей
    for (int row = startY; row <= endY; ++row) {
        for (int col = startX; col <= endX; ++col) {
            // Пропускаем клетки, которые принадлежат самому кораблю
            if (orientation == Orientation::Vertical && col == x && row >= y && row < y + shipSize) {
                continue;
            }
            if (orientation == Orientation::Horizontal && row == y && col >= x && col < x + shipSize) {
                continue;
            }

            if (field_[row][col] != CellStatus::Empty) {
                wxLogMessage("Невозможно разместить корабль: соседняя клетка занята на (%d, %d)", col, row);
                return false;
            }
        }
    }

    return true;
}

void Field::placeShip(int x, int y, int shipSize, Orientation orientation) {
    wxLogMessage("Размещение корабля на координатах (%d, %d) с размером %d и ориентацией %s",
                 x, y, shipSize, (orientation == Orientation::Vertical) ? "вертикально" : "горизонтально");

    if (orientation == Orientation::Vertical) {
        for (int i = 0; i < shipSize; ++i) {
            field_[y + i][x] = CellStatus::Ship;
        }
    } else { // Horizontal
        for (int i = 0; i < shipSize; ++i) {
            field_[y][x + i] = CellStatus::Ship;
        }
    }
}

bool Field::isCellOccupied(int x, int y) const {
    return field_[y][x] == CellStatus::Ship;
}

void Field::markCellAsHit(int x, int y) {
    if (field_[y][x] == CellStatus::Ship) {
        field_[y][x] = CellStatus::Hit;
    }
}

void Field::markCellAsMiss(int x, int y) {
    if (field_[y][x] == CellStatus::Empty) {
        field_[y][x] = CellStatus::Miss;
    }
}

bool Field::attackCell(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        wxLogMessage("Ошибка: координаты (%d, %d) выходят за пределы поля", x, y);
        return false;
    }

    if (field_[y][x] == CellStatus::Ship) {
        field_[y][x] = CellStatus::Hit;
        markCellAsHit(x, y);
        wxLogMessage("Попадание по кораблю на координатах (%d, %d)", x, y);
        return true;
    } else if (field_[y][x] == CellStatus::Empty) {
        field_[y][x] = CellStatus::Miss;
        markCellAsMiss(x, y);
        wxLogMessage("Промах по координатам (%d, %d)", x, y);
        return false;
    }

    wxLogMessage("Попадание в ранее атакованную клетку на координатах (%d, %d)", x, y);
    return false;
}

CellStatus Field::getCellStatus(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return CellStatus::Unknown;
    }
    return field_[y][x];
}

int Field::getWidth() const {
    return width_;
}

int Field::getHeight() const {
    return height_;
}
