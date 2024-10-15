#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include <vector>
#include "Ship.h"

class ShipManager {
public:
    ShipManager(const std::vector<std::pair<int, bool>>& shipData);

    // Метод для обработки атаки по кораблю
    bool attackShip(int shipIndex, int segmentIndex);

    // Метод для проверки, уничтожен ли корабль
    bool isShipDestroyed(int shipIndex) const;

    // Возвращает количество доступных кораблей определённого размера
    int getAvailableShips(int size) const;

    // Уменьшение количества кораблей определённого размера
    bool decreaseShipCount(int size);

private:
    std::vector<Ship> ships;  // Список всех кораблей
    std::vector<int> availableShips;  // Количество доступных кораблей каждого типа
};

#endif  // SHIPMANAGER_H
