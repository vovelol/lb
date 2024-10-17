// File: src/Ship.cpp
#include "Ship.h"

Ship::Ship(int size) : size_(size) {}

void Ship::setPosition(const std::vector<Coordinates>& positions) {
    cells_.clear();
    for (const auto& pos : positions) {
        cells_.emplace_back(pos.x, pos.y);
    }
}

bool Ship::hit(int x, int y) {
    for (auto& cell : cells_) {
        if (cell.coordinates.x == x && cell.coordinates.y == y) {
            if (cell.hp > 0) {
                cell.hp -= 1;
                return true; // Попадание
            }
        }
    }
    return false; // Мимо или уже уничтожен
}

bool Ship::isSunk() const {
    for (const auto& cell : cells_) {
        if (cell.hp > 0) {
            return false;
        }
    }
    return true;
}

int Ship::getSize() const {
    return size_;
}

const std::vector<ShipCell>& Ship::getCells() const {
    return cells_;
}

std::vector<ShipCell>& Ship::getCells() {
    return cells_;
}
