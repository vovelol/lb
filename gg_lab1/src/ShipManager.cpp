// File: src/ShipManager.cpp
#include "ShipManager.h"

ShipManager::ShipManager(int oneDeck, int twoDeck, int threeDeck, int fourDeck) {
    availableShips_[1] = oneDeck;
    availableShips_[2] = twoDeck;
    availableShips_[3] = threeDeck;
    availableShips_[4] = fourDeck;
}

void ShipManager::addShip(const Ship& ship) {
    ships_.emplace_back(ship);
}

std::vector<Ship>& ShipManager::getShips() {
    return ships_;
}

const std::vector<Ship>& ShipManager::getShips() const {
    return ships_;
}

void ShipManager::decreaseAvailableShips(int shipSize) {
    if (availableShips_.find(shipSize) != availableShips_.end() && availableShips_[shipSize] > 0) {
        availableShips_[shipSize]--;
    }
}

int ShipManager::getAvailableShips(int shipSize) const {
    auto it = availableShips_.find(shipSize);
    if (it != availableShips_.end()) {
        return it->second;
    }
    return 0;
}
