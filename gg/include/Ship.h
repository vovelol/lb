#ifndef SHIP_H
#define SHIP_H

class Ship {
public:
    Ship(int length, bool isVertical);

    bool hitSegment(int segmentIndex);  // Попадание в сегмент
    bool isSunk() const;  // Потоплен ли корабль
    int getLength() const;  // Получить длину корабля

private:
    int length_;
    bool isVertical_;
    int health_;  // Здоровье корабля (сколько сегментов осталось)
};

#endif // SHIP_H
