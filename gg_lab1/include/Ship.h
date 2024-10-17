// File: include/Ship.h
#ifndef SHIP_H
#define SHIP_H

#include <vector>

// Структура для координат клетки
struct Coordinates {
    int x;
    int y;

    bool operator==(const Coordinates& other) const {
        return x == other.x && y == other.y;
    }
};

// Структура для сегмента корабля
struct ShipCell {
    Coordinates coordinates;
    int hp; // Health Points: 2 - целый, 1 - поврежденный, 0 - уничтоженный

    ShipCell(int xCoord, int yCoord) : coordinates{ xCoord, yCoord }, hp(2) {} // Изначально 2 HP
};

class Ship {
public:
    Ship(int size);

    void setPosition(const std::vector<Coordinates>& positions);
    bool hit(int x, int y);       // Нанести удар по этому сегменту
    bool isSunk() const;          // Проверить, уничтожен ли корабль

    int getSize() const;
    const std::vector<ShipCell>& getCells() const;  // Для чтения
    std::vector<ShipCell>& getCells();              // Для модификации

private:
    int size_;
    std::vector<ShipCell> cells_;  // Клетки корабля и их состояния
};

#endif // SHIP_H
