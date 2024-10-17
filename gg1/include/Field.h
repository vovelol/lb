#ifndef FIELD_H
#define FIELD_H

#include "ShipManager.h"
#include <vector>

class Field {
public:
    Field(int width, int height, ShipManager& shipManager);

    void clear();
    bool canPlaceShip(int x, int y, int shipSize, bool isVertical) const;
    bool canPlaceShipWithNeighbors(int x, int y, int shipSize, bool isVertical) const;
    void placeShip(int x, int y, int shipSize, bool isVertical);
    bool isCellOccupied(int x, int y) const;
    void markCellAsHit(int x, int y);
    void markCellAsMiss(int x, int y);
    bool attackCell(int x, int y);

private:
    int width_;
    int height_;
    ShipManager& shipManager_;
    std::vector<std::vector<char>> field_; // Определяем поле как двумерный вектор символов
};

#endif // FIELD_H
