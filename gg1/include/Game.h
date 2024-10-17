#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "ShipManager.h"
#include "Ship.h"
#include <vector>

class Game {
public:
    Game(int width, int height, const std::vector<Ship>& ships);
    bool attackShip(int x, int y);
    void clearShipMarkers();
    bool canPlaceShip(int x, int y, int shipSize, bool isVertical);
    void placeShip(int x, int y, int shipSize, bool isVertical);
    void decreaseAvailableShips(int shipSize);
    int getAvailableShips(int shipSize) const;

private:
    Field field_;
    ShipManager shipManager_;

    int countShips(const std::vector<Ship>& ships, int shipSize) const;
};

#endif // GAME_H
