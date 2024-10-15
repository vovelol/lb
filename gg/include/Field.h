#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "ShipManager.h"

enum CellState {
    EMPTY,
    SHIP,
    HIT,
    MISS
};

class Field {
public:
    Field(int w, int h, ShipManager& manager);

    bool isCellOccupied(int x, int y) const;
    void markCellAsHit(int x, int y);
    void markCellAsMiss(int x, int y);
    void placeShip(int x, int y, int length, bool isVertical);
    bool canPlaceShip(int x, int y, int length, bool isVertical) const;  // Добавляем метод проверки

private:
    int width;
    int height;
    ShipManager& shipManager;
    std::vector<std::vector<CellState>> grid;
};

#endif  // FIELD_H
