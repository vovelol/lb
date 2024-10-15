#include "Game.h"

// Конструктор Game
Game::Game(int width, int height, const std::vector<std::pair<int, bool>>& shipData)
    : shipManager_(0, 0, 0, 0), field_(width, height, shipManager_) {

    int oneDeck = 0;
    int twoDeck = 0;
    int threeDeck = 0;
    int fourDeck = 0;

    // Подсчитаем количество каждого типа корабля из shipData
    for (const auto& ship : shipData) {
        switch (ship.first) {
            case 1: ++oneDeck; break;
            case 2: ++twoDeck; break;
            case 3: ++threeDeck; break;
            case 4: ++fourDeck; break;
            default: break;
        }
    }

    // Инициализируем shipManager_ с правильным количеством кораблей
    shipManager_ = ShipManager(oneDeck, twoDeck, threeDeck, fourDeck);
}

// Метод для получения ссылки на поле
Field& Game::getField() {
    return field_;
}

// Метод для получения ссылки на менеджера кораблей
ShipManager& Game::getShipManager() {
    return shipManager_;
}
