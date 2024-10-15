#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "ShipManager.h"
#include <vector>

class Game {
public:
    Game(int width, int height, const std::vector<std::pair<int, bool>>& shipData);

    void startGame();  // Начать игру
    void placeShip(int length, bool isVertical, int x, int y);  // Размещение корабля
    bool attack(int x, int y);  // Атака на клетку

    ShipManager& getShipManager();  // Получить менеджер кораблей
    Field& getField();  // Получить игровое поле

private:
    Field field_;  // Игровое поле
    ShipManager shipManager_;  // Менеджер кораблей
};

#endif  // GAME_H
