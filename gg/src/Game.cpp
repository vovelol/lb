#include "Game.h"

Game::Game(int width, int height, const std::vector<std::pair<int, bool>>& shipData)
    : field_(width, height, shipManager_), shipManager_(shipData) {}

void Game::startGame() {
    // Начало игры
}

void Game::placeShip(int length, bool isVertical, int x, int y) {
    field_.placeShip(x, y, length, isVertical);
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

ShipManager& Game::getShipManager() {
    return shipManager_;
}

Field& Game::getField() {
    return field_;
}
