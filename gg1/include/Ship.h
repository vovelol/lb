#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <utility>

struct ShipCell {
    std::pair<int, int> coordinates;
    bool isHit;
    ShipCell(int x, int y) : coordinates(x, y), isHit(false) {}
};

class Ship {
public:
    Ship(int size);

    void setPosition(const std::vector<std::pair<int, int>>& positions);
    bool isHit(int x, int y);  // Проверить, попал ли выстрел по этому кораблю
    bool isSunk() const;       // Проверить, потоплен ли весь корабль

    int getSize() const;
    std::vector<ShipCell>& getCells();  // Получить изменяемую ссылку на клетки корабля

private:
    int size_;
    std::vector<ShipCell> cells_;  // Клетки корабля и их состояния
};

#endif // SHIP_H
