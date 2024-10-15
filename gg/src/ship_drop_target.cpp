#include "ship_drop_target.h"
#include "app.h"

ShipDropTarget::ShipDropTarget(MyFrame* frame)
    : frame_(frame) {
    // Конструктор инициализирует указатель на MyFrame
}

bool ShipDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data) {
    int shipSize;
    if (wxSscanf(data, "Ship:%d", &shipSize) != 1) {
        wxLogMessage("Ошибка: неверный формат данных для корабля");
        return false;
    }

    int row = y / frame_->getGameGrid()->GetDefaultRowSize();
    int col = x / frame_->getGameGrid()->GetDefaultColSize();

    frame_->OnGridCellDrop(shipSize, col, row);
    return true;
}
