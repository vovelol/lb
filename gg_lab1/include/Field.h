// File: include/Field.h
#ifndef FIELD_H
#define FIELD_H

#include "common.h" // Для Orientation
#include "ShipManager.h"
#include <vector>
#include <algorithm>

enum class CellStatus {
    Unknown,
    Empty,
    Ship,
    Hit,
    Miss,
    Destroyed
};

class Field {
public:
    Field(int width, int height, ShipManager& shipManager);

    void clear();
    bool canPlaceShip(int x, int y, int shipSize, Orientation orientation) const;
    bool canPlaceShipWithNeighbors(int x, int y, int shipSize, Orientation orientation) const;
    void placeShip(int x, int y, int shipSize, Orientation orientation);
    bool isCellOccupied(int x, int y) const;
    void markCellAsHit(int x, int y);
    void markCellAsMiss(int x, int y);
    bool attackCell(int x, int y);

    // Метод для получения статуса клетки
    CellStatus getCellStatus(int x, int y) const;

    // Методы для получения размеров поля
    int getWidth() const;
    int getHeight() const;

private:
    int width_;
    int height_;
    ShipManager& shipManager_;
    std::vector<std::vector<CellStatus>> field_; // Двумерный вектор CellStatus
};

#endif // FIELD_H
