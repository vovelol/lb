#include "Ship.h"

Ship::Ship(int size) : size_(size) {}

void Ship::setPosition(const std::vector<std::pair<int, int>>& positions) {
    cells_.clear();
    for (const auto& pos : positions) {
        cells_.emplace_back(pos.first, pos.second);
    }
}

bool Ship::isHit(int x, int y) {
    for (auto& cell : cells_) {
        if (cell.coordinates.first == x && cell.coordinates.second == y) {
            cell.isHit = true;
            return true;
        }
    }
    return false;
}

bool Ship::isSunk() const {
    for (const auto& cell : cells_) {
        if (!cell.isHit) {
            return false;
        }
    }
    return true;
}

int Ship::getSize() const {
    return size_;
}

std::vector<ShipCell>& Ship::getCells() {
    return cells_;
}
