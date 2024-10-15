#include "Game.h"
#include "Field.h"

Game::Game(int width, int height, const std::vector<std::pair<int, bool>>& shipData)
    : shipManager_(shipData), field_(width, height, shipManager_) {
    // Здесь можно добавить дополнительную инициализацию
}

bool Game::attack(int x, int y) {
    if (field_.isCellOccupied(x, y)) {
        field_.markCellAsHit(x, y);
        return true;
    } else {
        field_.markCellAsMiss(x, y);
        return false;
    }
}

// Реализация метода для получения ссылки на ShipManager
ShipManager& Game::getShipManager() {
    return shipManager_;
}

// Реализация метода для получения ссылки на Field
Field& Game::getField() {
    return field_;
}
