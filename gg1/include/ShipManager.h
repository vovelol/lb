#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include "Ship.h"
#include <vector>

class ShipManager {
public:
    ShipManager(int oneDeck, int twoDeck, int threeDeck, int fourDeck);

    const std::vector<Ship>& getShips() const;  // Этот метод останется для случаев, когда нужен только доступ для чтения.
    std::vector<Ship>& getShips();  // Добавляем метод, возвращающий изменяемую ссылку для изменения кораблей.

    void addShip(const Ship& ship);
    void decreaseAvailableShips(int shipSize);
    int getAvailableShips(int shipSize) const;

private:
    std::vector<Ship> ships_;
    int oneDeck_;
    int twoDeck_;
    int threeDeck_;
    int fourDeck_;
};

#endif // SHIPMANAGER_H
