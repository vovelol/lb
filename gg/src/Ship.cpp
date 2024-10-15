#include "Ship.h"

Ship::Ship(int length, bool isVertical)
    : length_(length), isVertical_(isVertical), health_(length) {}

bool Ship::hitSegment(int segmentIndex) {
    if (segmentIndex < 0 || segmentIndex >= length_) {
        return false;  // Неверный индекс сегмента
    }
    health_--;
    return true;
}

bool Ship::isSunk() const {
    return health_ <= 0;
}

int Ship::getLength() const {
    return length_;
}
