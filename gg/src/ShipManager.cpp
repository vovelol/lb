#include "ShipManager.h"

ShipManager::ShipManager(const std::vector<std::pair<int, bool>>& shipData) {
    for (const auto& data : shipData) {
        ships.emplace_back(data.first, data.second);
        availableShips.push_back(data.first);  // Добавляем количество доступных кораблей
    }
}

// Обрабатываем попадание по кораблю
bool ShipManager::attackShip(int shipIndex, int segmentIndex) {
    if (shipIndex >= 0 && shipIndex < ships.size()) {
        return ships[shipIndex].hitSegment(segmentIndex);
    }
    return false;
}

// Проверяем, уничтожен ли корабль
bool ShipManager::isShipDestroyed(int shipIndex) const {
    if (shipIndex >= 0 && shipIndex < ships.size()) {
        return ships[shipIndex].isSunk();
    }
    return false;
}

// Возвращаем количество доступных кораблей определённого размера
int ShipManager::getAvailableShips(int size) const {
    int count = 0;
    for (const auto& ship : ships) {
        if (ship.getLength() == size) {
            count++;
        }
    }
    return count;
}

// Уменьшаем количество кораблей определённого размера
bool ShipManager::decreaseShipCount(int size) {
    for (auto it = ships.begin(); it != ships.end(); ++it) {
        if (it->getLength() == size) {
            ships.erase(it);  // Удаляем корабль из списка
            return true;
        }
    }
    return false;
}
