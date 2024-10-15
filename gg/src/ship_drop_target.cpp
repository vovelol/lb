#include "ship_drop_target.h"
#include "app.h"

ShipDropTarget::ShipDropTarget(MyFrame* frame)
    : frame_(frame) {}

bool ShipDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data) {
    long shipSize;
    if (!data.AfterFirst(':').ToLong(&shipSize)) {
        wxLogMessage("Ошибка: неверные данные перетаскивания.");
        return false;
    }

    // Преобразуем координаты окна в координаты сетки, используя метод getGameGrid()
    int row = y / frame_->getGameGrid()->GetDefaultRowSize();
    int col = x / frame_->getGameGrid()->GetDefaultColSize();

    // Проверяем возможность установки корабля и добавляем его на поле
    frame_->OnGridCellDrop(shipSize, col, row);
    return true;
}
