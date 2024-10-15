#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "ShipManager.h"

class Field {
public:
    Field(int width, int height, ShipManager& shipManager);

    bool canPlaceShip(int x, int y, int shipSize, bool isVertical) const;
    bool canPlaceShipWithNeighbors(int x, int y, int shipSize, bool isVertical) const;
    void placeShip(int x, int y, int shipSize, bool isVertical);

    bool isCellOccupied(int x, int y) const;
    void markCellAsHit(int x, int y);
    void markCellAsMiss(int x, int y);

private:
    int width_;
    int height_;
    std::vector<std::vector<char>> field_;
    ShipManager& shipManager_;
};

#endif // FIELD_H
