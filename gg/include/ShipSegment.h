#ifndef SHIP_SEGMENT_H
#define SHIP_SEGMENT_H

// Определение сегмента корабля
struct ShipSegment {
    int hp;  // Количество здоровья сегмента

    // Конструктор по умолчанию
    ShipSegment(int health = 2) : hp(health) {}
};

#endif // SHIP_SEGMENT_H
