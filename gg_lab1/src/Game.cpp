// File: src/Game.cpp
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

AttackOutcome Game::attackShip(int x, int y) {
    AttackOutcome outcome;
    outcome.result = AttackResult::Miss; // По умолчанию промах

    // Проходим по всем кораблям и проверяем каждую клетку
    for (auto& ship : shipManager_.getShips()) {  // Получаем не-const доступ
        for (auto& cell : ship.getCells()) {      // Получаем не-const доступ
            if (cell.coordinates.x == x && cell.coordinates.y == y) {
                if (cell.hp > 0) {
                    cell.hp -= 1;
                    wxLogMessage("Попадание по кораблю в координатах (%d, %d). Оставшиеся HP: %d", x, y, cell.hp);
                    if (cell.hp == 0) {
                        wxLogMessage("Сегмент корабля в координатах (%d, %d) уничтожен.", x, y);
                        // Проверка, потоплен ли весь корабль
                        if (ship.isSunk()) {
                            wxLogMessage("Корабль полностью уничтожен!");
                            // Помечаем окружающие клетки как промахи
                            std::vector<Coordinates> surroundingCells = getSurroundingCells(ship.getCells());
                            for (const auto& coord : surroundingCells) {
                                // Проверяем, что клетка не содержит корабль и ещё не атакована
                                if (field_.getCellStatus(coord.x, coord.y) == CellStatus::Empty) {
                                    field_.attackCell(coord.x, coord.y); // Помечаем как промах
                                    outcome.autoMissedCells.emplace_back(coord);
                                    wxLogMessage("Автоматическая пометка клетки (%d, %d) как промах.", coord.x, coord.y);
                                }
                            }
                        }
                    }
                    outcome.result = AttackResult::Hit; // Попадание
                    return outcome;
                } else {
                    wxLogMessage("Клетка (%d, %d) уже уничтожена.", x, y);
                    outcome.result = AttackResult::AlreadyHitShip; // Уже уничтожен сегмент корабля
                    return outcome;
                }
            }
        }
    }

    // Если нет корабля в клетке, проверяем, был ли уже промах
    CellStatus status = field_.getCellStatus(x, y);
    if (status == CellStatus::Miss) {
        wxLogMessage("Клетка (%d, %d) уже была промахом.", x, y);
        outcome.result = AttackResult::AlreadyMissed;
        return outcome;
    }

    // Совершаем промах
    bool isHit = field_.attackCell(x, y); // true - Hit, false - Miss
    if (!isHit) {
        wxLogMessage("Промах по координатам (%d, %d)", x, y);
        outcome.result = AttackResult::Miss;
    } else {
        // В случае, если корабль был случайно установлен на этой клетке после предыдущего промаха
        outcome.result = AttackResult::Hit;
    }

    return outcome;
}

void Game::clearShipMarkers() {
    for (auto& ship : shipManager_.getShips()) {  // Получаем не-const доступ
        for (auto& cell : ship.getCells()) {      // Получаем не-const доступ
            cell.hp = 2; // Сбрасываем HP до 2 (целый сегмент)
        }
    }
    field_.clear();
}

bool Game::canPlaceShip(int x, int y, int shipSize, Orientation orientation) const {
    return field_.canPlaceShipWithNeighbors(x, y, shipSize, orientation);
}

void Game::placeShip(int x, int y, int shipSize, Orientation orientation) {
    wxLogMessage("Попытка размещения корабля с размером %d на координатах (%d, %d) с ориентацией %s",
                 shipSize, x, y, (orientation == Orientation::Vertical) ? "вертикально" : "горизонтально");

    if (field_.canPlaceShipWithNeighbors(x, y, shipSize, orientation)) {
        field_.placeShip(x, y, shipSize, orientation);
        shipManager_.decreaseAvailableShips(shipSize);
        wxLogMessage("Корабль успешно размещен на координатах (%d, %d)", x, y);

        // Устанавливаем позицию для первого доступного корабля соответствующего размера
        for (auto& ship : shipManager_.getShips()) {
            // Проверяем, что корабль нужного размера и еще не имеет установленных позиций
            if (ship.getSize() == shipSize && ship.getCells().empty()) {
                std::vector<Coordinates> positions;
                if (orientation == Orientation::Vertical) {
                    for (int i = 0; i < shipSize; ++i) {
                        positions.emplace_back(Coordinates{ x, y + i });
                    }
                } else { // Horizontal
                    for (int i = 0; i < shipSize; ++i) {
                        positions.emplace_back(Coordinates{ x + i, y });
                    }
                }
                ship.setPosition(positions);
                wxLogMessage("Корабль размером %d установлен на координатах (%d, %d)", shipSize, x, y);
                break; // Установили позиции для одного корабля, выходим из цикла
            }
        }
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

int Game::getCellHP(int x, int y) const {
    // Проходим по всем кораблям и ищем клетку
    for (const auto& ship : shipManager_.getShips()) {
        for (const auto& cell : ship.getCells()) {
            if (cell.coordinates.x == x && cell.coordinates.y == y) {
                return cell.hp;
            }
        }
    }
    return 0; // Если клетка не содержит корабля или уже уничтожена
}

std::vector<Coordinates> Game::getSurroundingCells(const std::vector<ShipCell>& shipCells) const {
    std::vector<Coordinates> surrounding;
    for (const auto& cell : shipCells) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int newX = cell.coordinates.x + dx;
                int newY = cell.coordinates.y + dy;
                // Проверяем, что координаты находятся внутри поля
                if (newX >= 0 && newX < field_.getWidth() && newY >= 0 && newY < field_.getHeight()) {
                    Coordinates coord = { newX, newY };
                    // Добавляем клетку, если её ещё нет в списке и не является частью корабля
                    if (std::find(surrounding.begin(), surrounding.end(), coord) == surrounding.end()) {
                        bool isPartOfShip = false;
                        for (const auto& otherShip : shipManager_.getShips()) {
                            for (const auto& otherCell : otherShip.getCells()) {
                                if (otherCell.coordinates.x == newX && otherCell.coordinates.y == newY) {
                                    isPartOfShip = true;
                                    break;
                                }
                            }
                            if (isPartOfShip) break;
                        }
                        if (!isPartOfShip) {
                            surrounding.emplace_back(coord);
                        }
                    }
                }
            }
        }
    }
    return surrounding;
}
