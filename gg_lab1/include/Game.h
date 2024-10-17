#ifndef GAME_H
#define GAME_H

#include "common.h" // Для Orientation
#include "Field.h"
#include "ShipManager.h"
#include "Ship.h"
#include <vector>

// Перечисление для результатов атаки
enum class AttackResult {
    Hit,                // Попадание
    Miss,               // Промах
    AlreadyHitShip,     // Уже уничтожен сегмент корабля
    AlreadyMissed       // Уже был промах
};

// Структура для результатов атаки с дополнительной информацией
struct AttackOutcome {
    AttackResult result;
    std::vector<Coordinates> autoMissedCells;
};

class Game {
public:
    Game(int width, int height, const std::vector<Ship>& ships);

    // Изменённый метод возвращает AttackOutcome
    AttackOutcome attackShip(int x, int y);          // Нанести удар по координате (x, y)
    void clearShipMarkers();                        // Сбросить состояния кораблей
    bool canPlaceShip(int x, int y, int shipSize, Orientation orientation) const;
    void placeShip(int x, int y, int shipSize, Orientation orientation);
    void decreaseAvailableShips(int shipSize);
    int getAvailableShips(int shipSize) const;

    int getCellHP(int x, int y) const;              // Метод для получения HP клетки

private:
    Field field_;
    ShipManager shipManager_;

    int countShips(const std::vector<Ship>& ships, int shipSize) const;

    // Метод для получения окружающих клеток
    std::vector<Coordinates> getSurroundingCells(const std::vector<ShipCell>& shipCells) const;
};

#endif // GAME_H
