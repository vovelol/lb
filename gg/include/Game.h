#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "ShipManager.h"
#include <vector>

class Game {
public:
    Game(int width, int height, const std::vector<std::pair<int, bool>>& shipData);

    // Методы для доступа к объектам поля и менеджера кораблей
    Field& getField();
    ShipManager& getShipManager();

private:
    ShipManager shipManager_;
    Field field_;
};

#endif  // GAME_H
