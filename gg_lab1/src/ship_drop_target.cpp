#include "ship_drop_target.h"
#include "app.h" // Включаем для определения MyFrame
#include <wx/log.h>
#include <wx/strconv.h>

ShipDropTarget::ShipDropTarget(MyFrame* frame) : frame_(frame) {}

bool ShipDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data) {
    if (!frame_) {
        wxLogError("Ошибка: frame не инициализирован.");
        return false;
    }

    int shipSize = 0;
    wxString remainingData;

    // Попробуем получить размер корабля из текста "Ship:<размер>"
    if (data.StartsWith("Ship:", &remainingData)) {
        long tempSize;
        if (remainingData.ToLong(&tempSize)) {
            shipSize = static_cast<int>(tempSize);
        } else {
            wxLogError("Ошибка: неверный формат данных о корабле.");
            return false;
        }
    } else {
        wxLogError("Ошибка: данные о корабле не начинаются с 'Ship:'.");
        return false;
    }

    // Определение координат сетки
    int col = x / frame_->getGameGrid()->GetDefaultColSize();
    int row = y / frame_->getGameGrid()->GetDefaultRowSize();

    wxLogMessage("Попытка размещения корабля с размером %d на координатах (%d, %d)", shipSize, col, row);

    // Вызов метода размещения корабля
    frame_->OnGridCellDrop(shipSize, col, row);
    return true;
}