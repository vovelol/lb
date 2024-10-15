#ifndef SHIP_H
#define SHIP_H

class Ship {
public:
    Ship(int size, bool orientation);

private:
    int size_;
    bool isVertical_;
};

#endif // SHIP_H
