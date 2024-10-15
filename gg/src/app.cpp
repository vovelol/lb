#include "app.h"
#include "settings_dialog.h"
#include "ship_drop_target.h"  // Включаем ship_drop_target.h для использования класса ShipDropTarget
#include <wx/dnd.h>
#include <wx/log.h>
#include <wx/wx.h>

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    
    mainSizer = new wxBoxSizer(wxHORIZONTAL);
    rightSizer = new wxBoxSizer(wxVERTICAL);

    // Подготавливаем данные для кораблей (размеры и количество)
    std::vector<std::pair<int, bool>> shipData = {
        {1, true},  // Один 1-палубный корабль
        {2, true},  // Два 2-палубных корабля
        {3, true},  // Три 3-палубных корабля
        {4, true}   // Четыре 4-палубных корабля
    };

    // Создаем объект Game с указанием размеров игрового поля и данных о кораблях
    game_ = std::make_unique<Game>(10, 10, shipData);

    // Инициализируем игровое поле
    InitializeGrid(10, 10, 1, 2, 3, 4); // Размер поля 10x10 и количество кораблей

    AddShipImages();

    // Назначаем цель для перетаскивания на игровое поле
    shipDropTarget = new ShipDropTarget(this);
    if (gameGrid != nullptr) {
        gameGrid->SetDropTarget(shipDropTarget);
    }

    SetSizer(mainSizer);
    Layout();
}

MyFrame::~MyFrame() {
    // Убедитесь, что освобождение памяти shipDropTarget выполняется корректно
    if (shipDropTarget != nullptr) {
        delete shipDropTarget;
    }
}

wxGrid* MyFrame::getGameGrid() const {
    return gameGrid;
}

void MyFrame::setGame(std::unique_ptr<Game> game) {
    game_ = std::move(game);
}

void MyFrame::InitializeGrid(int rows, int cols, int oneDeck, int twoDeck, int threeDeck, int fourDeck) {
    // Убедитесь, что игровое поле создается только один раз
    if (gameGrid == nullptr) {
        gameGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
        gameGrid->CreateGrid(rows, cols);
        gameGrid->EnableEditing(false);
        gameGrid->SetDefaultColSize(30, true);
        gameGrid->SetDefaultRowSize(30, true);

        // Добавляем игровую сетку в основное окно только один раз
        mainSizer->Add(gameGrid, 1, wxEXPAND | wxALL, 5);
        mainSizer->Add(rightSizer, 0, wxEXPAND | wxALL, 5);
    }

    Layout();
}

void MyFrame::AddShipImages() {
    // Загружаем изображения кораблей
    oneDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/one_deck.jpg"), wxBITMAP_TYPE_JPEG));
    twoDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/two_deck.jpg"), wxBITMAP_TYPE_JPEG));
    threeDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/three_deck.jpg"), wxBITMAP_TYPE_JPEG));
    fourDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/four_deck.jpg"), wxBITMAP_TYPE_JPEG));

    // Создаем текстовые поля для отображения количества оставшихся кораблей
    oneDeckShipText = new wxStaticText(this, wxID_ANY, wxT("x1"));
    twoDeckShipText = new wxStaticText(this, wxID_ANY, wxT("x2"));
    threeDeckShipText = new wxStaticText(this, wxID_ANY, wxT("x3"));
    fourDeckShipText = new wxStaticText(this, wxID_ANY, wxT("x4"));

    // Добавляем изображения и текст в правую панель
    rightSizer->Add(oneDeckShipImg, 0, wxALL, 5);
    rightSizer->Add(oneDeckShipText, 0, wxALL, 5);
    rightSizer->Add(twoDeckShipImg, 0, wxALL, 5);
    rightSizer->Add(twoDeckShipText, 0, wxALL, 5);
    rightSizer->Add(threeDeckShipImg, 0, wxALL, 5);
    rightSizer->Add(threeDeckShipText, 0, wxALL, 5);
    rightSizer->Add(fourDeckShipImg, 0, wxALL, 5);
    rightSizer->Add(fourDeckShipText, 0, wxALL, 5);

    // Привязываем события перетаскивания и переворота к изображениям
    BindShipEvents(oneDeckShipImg, 1);
    BindShipEvents(twoDeckShipImg, 2);
    BindShipEvents(threeDeckShipImg, 3);
    BindShipEvents(fourDeckShipImg, 4);
}

void MyFrame::BindShipEvents(wxStaticBitmap* shipImg, int shipSize) {
    // Привязываем события для перетаскивания и переворота
    shipImg->Bind(wxEVT_LEFT_DOWN, [this, shipImg, shipSize](wxMouseEvent& event) {
        if (game_ && game_->getShipManager().getAvailableShips(shipSize) <= 0) {
            wxLogMessage("Ошибка: нельзя разместить больше кораблей этого типа!");
            return;
        }

        wxTextDataObject data(wxString::Format("Ship:%d", shipSize));
        wxDropSource dragSource(shipImg);
        dragSource.SetData(data);
        wxDragResult result = dragSource.DoDragDrop(wxDrag_CopyOnly);
    });

    shipImg->Bind(wxEVT_RIGHT_DOWN, [this, shipImg, shipSize](wxMouseEvent& event) {
        ToggleShipOrientation(shipImg, shipSize);
    });
}

void MyFrame::OnGridCellDrop(int shipSize, int x, int y) {
    // Проверяем, возможно ли разместить корабль с учетом соседства
    if (!game_ || !game_->getField().canPlaceShipWithNeighbors(x, y, shipSize, isVertical)) {
        wxLogMessage("Ошибка: нельзя размещать корабли так близко друг к другу!");
        return;
    }

    // Если размещение возможно, добавляем корабль на игровое поле
    game_->getField().placeShip(x, y, shipSize, isVertical);

    // Обновляем графическое отображение сетки
    if (isVertical) {
        for (int i = 0; i < shipSize; ++i) {
            // Проверяем, что индексы находятся в допустимых границах
            if ((y + i) < gameGrid->GetNumberRows()) {
                gameGrid->SetCellValue(y + i, x, "X");  // Обозначаем корабль символом "X"
                gameGrid->SetCellBackgroundColour(y + i, x, *wxLIGHT_GREY);  // Меняем цвет ячейки для визуализации
            }
        }
    } else {
        for (int i = 0; i < shipSize; ++i) {
            // Проверяем, что индексы находятся в допустимых границах
            if ((x + i) < gameGrid->GetNumberCols()) {
                gameGrid->SetCellValue(y, x + i, "X");  // Обозначаем корабль символом "X"
                gameGrid->SetCellBackgroundColour(y, x + i, *wxLIGHT_GREY);  // Меняем цвет ячейки для визуализации
            }
        }
    }

    // Обновляем количество оставшихся кораблей
    UpdateShipCounters();

    // Обновляем графическое отображение сетки
    gameGrid->ForceRefresh();
}

void MyFrame::UpdateShipCounters() {
    // Обновляем счетчики кораблей
    oneDeckShipText->SetLabel(wxString::Format("x%d", game_->getShipManager().getAvailableShips(1)));
    twoDeckShipText->SetLabel(wxString::Format("x%d", game_->getShipManager().getAvailableShips(2)));
    threeDeckShipText->SetLabel(wxString::Format("x%d", game_->getShipManager().getAvailableShips(3)));
    fourDeckShipText->SetLabel(wxString::Format("x%d", game_->getShipManager().getAvailableShips(4)));
}

void MyFrame::ToggleShipOrientation(wxStaticBitmap* shipImg, int shipSize) {
    // Переворачиваем изображение на 90 градусов
    isVertical = !isVertical;

    wxImage image = shipImg->GetBitmap().ConvertToImage();
    image = image.Rotate90(isVertical);

    shipImg->SetBitmap(wxBitmap(image));
    wxLogMessage("Изменена ориентация корабля");
}

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // Здесь можно привязать события, если нужно
wxEND_EVENT_TABLE()
