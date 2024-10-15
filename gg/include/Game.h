#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Field.h"
#include "ShipManager.h"

class Game {
public:
    Game(int width, int height, const std::vector<std::pair<int, bool>>& shipData);

    bool attack(int x, int y);
    ShipManager& getShipManager();  // Метод для получения ссылки на ShipManager
    Field& getField();  // Метод для получения ссылки на Field

private:
    Field field_;
    ShipManager shipManager_;
};

#endif // GAME_H
