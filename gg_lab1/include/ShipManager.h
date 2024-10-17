// File: include/ShipManager.h
#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include "Ship.h"
#include <vector>
#include <map>

class ShipManager {
public:
    ShipManager(int oneDeck, int twoDeck, int threeDeck, int fourDeck);

    void addShip(const Ship& ship);
    std::vector<Ship>& getShips();
    const std::vector<Ship>& getShips() const;

    void decreaseAvailableShips(int shipSize);
    int getAvailableShips(int shipSize) const;

private:
    std::vector<Ship> ships_;
    std::map<int, int> availableShips_; // key: ship size, value: remaining ships
};

#endif // SHIPMANAGER_H
