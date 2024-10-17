#include "ShipManager.h"

ShipManager::ShipManager(int oneDeck, int twoDeck, int threeDeck, int fourDeck)
        : oneDeck_(oneDeck), twoDeck_(twoDeck), threeDeck_(threeDeck), fourDeck_(fourDeck) {}

const std::vector<Ship>& ShipManager::getShips() const {
    return ships_;
}

std::vector<Ship>& ShipManager::getShips() {
    return ships_;
}

void ShipManager::addShip(const Ship& ship) {
    ships_.push_back(ship);
}

void ShipManager::decreaseAvailableShips(int shipSize) {
    switch (shipSize) {
        case 1:
            --oneDeck_;
            break;
        case 2:
            --twoDeck_;
            break;
        case 3:
            --threeDeck_;
            break;
        case 4:
            --fourDeck_;
            break;
    }
}

int ShipManager::getAvailableShips(int shipSize) const {
    switch (shipSize) {
        case 1:
            return oneDeck_;
        case 2:
            return twoDeck_;
        case 3:
            return threeDeck_;
        case 4:
            return fourDeck_;
        default:
            return 0;
    }
}
