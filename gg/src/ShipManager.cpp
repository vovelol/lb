#include "ShipManager.h"

ShipManager::ShipManager(int oneDeck, int twoDeck, int threeDeck, int fourDeck) {
    availableShips[1] = oneDeck;
    availableShips[2] = twoDeck;
    availableShips[3] = threeDeck;
    availableShips[4] = fourDeck;
}

int ShipManager::getAvailableShips(int size) const {
    auto it = availableShips.find(size);
    if (it != availableShips.end()) {
        return it->second;
    }
    return 0; // Если такого размера корабля нет, возвращаем 0
}

void ShipManager::decreaseAvailableShips(int size) {
    auto it = availableShips.find(size);
    if (it != availableShips.end() && it->second > 0) {
        it->second -= 1; // Уменьшаем количество доступных кораблей на 1, если оно больше 0
    }
}
