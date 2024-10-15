#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include <map>

class ShipManager {
public:
    ShipManager(int oneDeck, int twoDeck, int threeDeck, int fourDeck);
    
    int getAvailableShips(int size) const;
    void decreaseAvailableShips(int size);

private:
    std::map<int, int> availableShips; // Ключ — размер корабля, значение — количество доступных кораблей
};

#endif // SHIPMANAGER_H
