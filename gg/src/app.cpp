#include "app.h"
#include "settings_dialog.h"
#include "ship_drop_target.h"  // Не забудьте добавить этот include
#include <wx/dnd.h>  // Добавляем этот заголовок для поддержки drag-and-drop
#include <wx/log.h>
#include <wx/wx.h>

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
    mainSizer = new wxBoxSizer(wxHORIZONTAL);
    rightSizer = new wxBoxSizer(wxVERTICAL);

    // Инициализируем игровое поле
    InitializeGrid(10, 10, 1, 2, 3, 4); // Инициализация с размерами и количеством кораблей

    AddShipImages();

    // Устанавливаем цель для перетаскивания на сетку
    shipDropTarget = new ShipDropTarget(this);
    gameGrid->SetDropTarget(shipDropTarget);

    SetSizer(mainSizer);
}

MyFrame::~MyFrame() {
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
    gameGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    gameGrid->CreateGrid(rows, cols);
    gameGrid->EnableEditing(false);
    gameGrid->SetDefaultColSize(30, true);
    gameGrid->SetDefaultRowSize(30, true);

    // Назначаем ShipDropTarget в качестве цели для игрового поля
    shipDropTarget = new ShipDropTarget(this);
    gameGrid->SetDropTarget(shipDropTarget);

    mainSizer->Add(gameGrid, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(rightSizer, 0, wxEXPAND | wxALL, 5);

    Layout();
}

void MyFrame::AddShipImages() {
    oneDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/one_deck.jpg"), wxBITMAP_TYPE_JPEG));
    twoDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/two_deck.jpg"), wxBITMAP_TYPE_JPEG));
    threeDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/three_deck.jpg"), wxBITMAP_TYPE_JPEG));
    fourDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/four_deck.jpg"), wxBITMAP_TYPE_JPEG));

    oneDeckShipText = new wxStaticText(this, wxID_ANY, wxT("x1"));
    twoDeckShipText = new wxStaticText(this, wxID_ANY, wxT("x2"));
    threeDeckShipText = new wxStaticText(this, wxID_ANY, wxT("x3"));
    fourDeckShipText = new wxStaticText(this, wxID_ANY, wxT("x4"));

    rightSizer->Add(oneDeckShipImg, 0, wxALL, 5);
    rightSizer->Add(oneDeckShipText, 0, wxALL, 5);
    rightSizer->Add(twoDeckShipImg, 0, wxALL, 5);
    rightSizer->Add(twoDeckShipText, 0, wxALL, 5);
    rightSizer->Add(threeDeckShipImg, 0, wxALL, 5);
    rightSizer->Add(threeDeckShipText, 0, wxALL, 5);
    rightSizer->Add(fourDeckShipImg, 0, wxALL, 5);
    rightSizer->Add(fourDeckShipText, 0, wxALL, 5);
}

void MyFrame::BindShipEvents(wxStaticBitmap* shipImg, int shipSize) {
    shipImg->Bind(wxEVT_LEFT_DOWN, [this, shipImg, shipSize](wxMouseEvent& event) {
        if (game_->getShipManager().getAvailableShips(shipSize) <= 0) {
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
    if (!game_->getField().canPlaceShip(x, y, shipSize, isVertical)) {
        wxLogMessage("Ошибка: нельзя разместить корабль здесь!");
        return;
    }

    game_->getField().placeShip(x, y, shipSize, isVertical);
    UpdateShipCounters();
}

void MyFrame::OnGridCellClick(wxGridEvent& gridEvent) {
    int row = gridEvent.GetRow();
    int col = gridEvent.GetCol();
    // Логика клика по клетке игрового поля
    wxLogMessage(wxString::Format("Clicked on cell: (%d, %d)", row, col));
}

void MyFrame::OnGridAttackClick(wxGridEvent& gridEvent) {
    int row = gridEvent.GetRow();
    int col = gridEvent.GetCol();

    // Здесь будет логика атаки по клетке
    wxLogMessage(wxString::Format("Атака по клетке [%d, %d]", row, col));
}

void MyFrame::ShowSettingsDialog() {
    // Открытие окна настроек
    wxLogMessage("Открытие окна настроек");
}

void MyFrame::UpdateShipCounters() {
    oneDeckShipText->SetLabel(wxString::Format("x%d", game_->getShipManager().getAvailableShips(1)));
    twoDeckShipText->SetLabel(wxString::Format("x%d", game_->getShipManager().getAvailableShips(2)));
    threeDeckShipText->SetLabel(wxString::Format("x%d", game_->getShipManager().getAvailableShips(3)));
    fourDeckShipText->SetLabel(wxString::Format("x%d", game_->getShipManager().getAvailableShips(4)));
}

void MyFrame::DecreaseShipCount(int shipSize) {
    game_->getShipManager().decreaseShipCount(shipSize);
}

void MyFrame::ToggleShipOrientation(wxStaticBitmap* shipImg, int shipSize) {
    // Переворачиваем изображение на 90 градусов
    isVertical = !isVertical;

    wxImage image = shipImg->GetBitmap().ConvertToImage();
    if (isVertical) {
        image = image.Rotate90(true);  // Поворот на 90 градусов по часовой стрелке
    } else {
        image = image.Rotate90(false);  // Поворот на 90 градусов против часовой стрелки
    }

    shipImg->SetBitmap(wxBitmap(image));
    wxLogMessage("Изменена ориентация корабля");
}

void MyFrame::OnStartGame(wxCommandEvent& event) {
    // Логика старта игры
    wxLogMessage("Игра началась");
}

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    // Привяжите события здесь, если нужно
wxEND_EVENT_TABLE()
