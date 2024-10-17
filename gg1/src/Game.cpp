#include "Game.h"
#include <wx/log.h>

Game::Game(int width, int height, const std::vector<Ship>& ships)
        : shipManager_(countShips(ships, 1), countShips(ships, 2), countShips(ships, 3), countShips(ships, 4)),
          field_(width, height, shipManager_) {
    // Добавляем все корабли в менеджер
    for (const auto& ship : ships) {
        shipManager_.addShip(ship);
    }
}

bool Game::attackShip(int x, int y) {
    // Проходим по всем кораблям и проверяем каждую клетку
    for (auto& ship : shipManager_.getShips()) {
        for (auto& cell : ship.getCells()) {
            if (cell.coordinates.first == x && cell.coordinates.second == y) {
                if (!cell.isHit) {
                    cell.isHit = true;
                    wxLogMessage("Попадание по кораблю в координатах (%d, %d)", x, y);
                    return true; // Попали в корабль
                } else {
                    wxLogMessage("Повторное попадание в ту же клетку (%d, %d)", x, y);
                    return false; // Уже пораженная клетка
                }
            }
        }
    }
    wxLogMessage("Мимо в координатах (%d, %d)", x, y);
    return false; // Если ни один корабль не найден, это промах
}

void Game::clearShipMarkers() {
    for (auto& ship : shipManager_.getShips()) {
        for (auto& cell : ship.getCells()) {
            cell.isHit = false; // Сбрасываем состояние попадания
        }
    }
    field_.clear();
}

bool Game::canPlaceShip(int x, int y, int shipSize, bool isVertical) {
    return field_.canPlaceShipWithNeighbors(x, y, shipSize, isVertical);
}

void Game::placeShip(int x, int y, int shipSize, bool isVertical) {
    wxLogMessage("Попытка размещения корабля с размером %d на координатах (%d, %d) с ориентацией %s",
                 shipSize, x, y, isVertical ? "вертикально" : "горизонтально");

    if (field_.canPlaceShipWithNeighbors(x, y, shipSize, isVertical)) {
        field_.placeShip(x, y, shipSize, isVertical);
        wxLogMessage("Корабль успешно размещен на координатах (%d, %d)", x, y);
    } else {
        wxLogMessage("Ошибка: нельзя размещать корабли так близко друг к другу на координатах (%d, %d)", x, y);
    }
}

void Game::decreaseAvailableShips(int shipSize) {
    shipManager_.decreaseAvailableShips(shipSize);
}

int Game::getAvailableShips(int shipSize) const {
    return shipManager_.getAvailableShips(shipSize);
}

int Game::countShips(const std::vector<Ship>& ships, int shipSize) const {
    int count = 0;
    for (const Ship& ship : ships) {
        if (ship.getSize() == shipSize) {
            ++count;
        }
    }
    return count;
}
