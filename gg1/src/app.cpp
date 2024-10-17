#include "app.h"
#include "settings_dialog.h"  // Включаем заголовочный файл диалога настроек
#include "ship_drop_target.h"
#include <wx/log.h>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <memory>
#include <map>  // Добавлено для использования std::map

enum {
    ID_START_BUTTON = wxID_HIGHEST + 1,
};

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, title, pos, size), shipDropTarget(nullptr), remainingOneDeck(0), remainingTwoDeck(0), remainingThreeDeck(0), remainingFourDeck(0) {

    mainSizer = new wxBoxSizer(wxHORIZONTAL); // Горизонтальный sizer для выравнивания элементов
    controlSizer = new wxBoxSizer(wxVERTICAL); // Вертикальный sizer для панели управления

    // Вызов диалога настроек
    SettingsDialog settingsDialog(this);
    if (settingsDialog.ShowModal() == wxID_OK) {
        gridSize = settingsDialog.getGridSize();
        oneDeck = settingsDialog.getOneDeckShips();
        twoDeck = settingsDialog.getTwoDeckShips();
        threeDeck = settingsDialog.getThreeDeckShips();
        fourDeck = settingsDialog.getFourDeckShips();

        remainingOneDeck = oneDeck;
        remainingTwoDeck = twoDeck;
        remainingThreeDeck = threeDeck;
        remainingFourDeck = fourDeck;

        // Инициализация ориентаций для всех кораблей (по умолчанию горизонтально)
        shipOrientations[1] = false;
        shipOrientations[2] = false;
        shipOrientations[3] = false;
        shipOrientations[4] = false;

        StartGame();
    }

    // Привязка события нажатия кнопки
    Bind(wxEVT_BUTTON, &MyFrame::OnStartButtonClicked, this, ID_START_BUTTON);
}

MyFrame::~MyFrame() {
    // Деструктор
}

wxGrid* MyFrame::getGameGrid() const {
    return gameGrid;
}

void MyFrame::StartGame() {
    // Создаем вектор данных кораблей
    std::vector<Ship> shipData;
    for (int i = 0; i < oneDeck; ++i) shipData.emplace_back(1);
    for (int i = 0; i < twoDeck; ++i) shipData.emplace_back(2);
    for (int i = 0; i < threeDeck; ++i) shipData.emplace_back(3);
    for (int i = 0; i < fourDeck; ++i) shipData.emplace_back(4);

    // Создаем игру
    try {
        game_ = std::make_unique<Game>(gridSize, gridSize, shipData);
    } catch (const std::exception& e) {
        wxLogError("Ошибка при создании игры: %s", e.what());
        return;
    }

    // Инициализируем игровое поле
    InitializeGrid(gridSize);

    // Добавляем изображения кораблей
    AddShipImages();

    // Создаем цель для перетаскивания
    shipDropTarget = new ShipDropTarget(this);
    if (!shipDropTarget || !gameGrid) {
        wxLogError("Ошибка: не удалось инициализировать цель для перетаскивания.");
        return;
    }
    gameGrid->SetDropTarget(shipDropTarget);

    // Создаем кнопку для начала игры после размещения кораблей
    startButton = new wxButton(this, ID_START_BUTTON, "Начать Игру");
    if (!startButton) {
        wxLogError("Ошибка: не удалось создать кнопку 'Начать Игру'.");
        return;
    }

    // Добавляем sizers в mainSizer
    controlSizer->Add(startButton, 0, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(gameGrid, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(controlSizer, 0, wxEXPAND | wxALL, 10);

    SetSizerAndFit(mainSizer);
    Layout();
}

void MyFrame::OnStartButtonClicked(wxCommandEvent& event) {
    wxLogMessage("Игра началась! Теперь можно атаковать!");

    // Очищаем визуальное представление кораблей на поле, оставляя их данные в памяти
    gameGrid->ClearGrid();
    ClearGridColors();
    game_->clearShipMarkers();

    // Добавляем игровое поле для атаки
    gameGrid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &MyFrame::OnGridAttackClick, this);

    Layout();
}

void MyFrame::OnGridAttackClick(wxGridEvent& gridEvent) {
    int row = gridEvent.GetRow();
    int col = gridEvent.GetCol();

    // Проверяем попадание по кораблю
    if (game_ && game_->attackShip(row, col)) {
        wxLogMessage("Попадание!");
        gameGrid->SetCellValue(row, col, "X");
        gameGrid->SetCellBackgroundColour(row, col, *wxRED);
    } else {
        wxLogMessage("Мимо!");
        gameGrid->SetCellValue(row, col, "O");
        gameGrid->SetCellBackgroundColour(row, col, *wxBLUE);
    }
    gameGrid->ForceRefresh();
}

void MyFrame::InitializeGrid(int size) {
    gameGrid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    if (!gameGrid) {
        wxLogError("Ошибка: не удалось создать игровую сетку.");
        return;
    }
    gameGrid->CreateGrid(size, size);
    gameGrid->EnableEditing(false);
    gameGrid->SetDefaultColSize(30, true);
    gameGrid->SetDefaultRowSize(30, true);
}

void MyFrame::AddShipImages() {
    if (!controlSizer) {
        controlSizer = new wxBoxSizer(wxVERTICAL);
        mainSizer->Add(controlSizer, 0, wxEXPAND | wxALL, 10);
    }

    try {
        oneDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/one_deck.jpg"), wxBITMAP_TYPE_JPEG));
        if (!oneDeckShipImg) throw std::runtime_error("Ошибка: не удалось создать изображение для однопалубного корабля");
        controlSizer->Add(oneDeckShipImg, 0, wxALL, 5);
        BindShipEvents(oneDeckShipImg, 1);
        oneDeckShipText = new wxStaticText(this, wxID_ANY, wxString::Format("x%d", remainingOneDeck));
        controlSizer->Add(oneDeckShipText, 0, wxALL, 5);

        twoDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/two_deck.jpg"), wxBITMAP_TYPE_JPEG));
        if (!twoDeckShipImg) throw std::runtime_error("Ошибка: не удалось создать изображение для двухпалубного корабля");
        controlSizer->Add(twoDeckShipImg, 0, wxALL, 5);
        BindShipEvents(twoDeckShipImg, 2);
        twoDeckShipText = new wxStaticText(this, wxID_ANY, wxString::Format("x%d", remainingTwoDeck));
        controlSizer->Add(twoDeckShipText, 0, wxALL, 5);

        threeDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/three_deck.jpg"), wxBITMAP_TYPE_JPEG));
        if (!threeDeckShipImg) throw std::runtime_error("Ошибка: не удалось создать изображение для трехпалубного корабля");
        controlSizer->Add(threeDeckShipImg, 0, wxALL, 5);
        BindShipEvents(threeDeckShipImg, 3);
        threeDeckShipText = new wxStaticText(this, wxID_ANY, wxString::Format("x%d", remainingThreeDeck));
        controlSizer->Add(threeDeckShipText, 0, wxALL, 5);

        fourDeckShipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxT("images/four_deck.jpg"), wxBITMAP_TYPE_JPEG));
        if (!fourDeckShipImg) throw std::runtime_error("Ошибка: не удалось создать изображение для четырехпалубного корабля");
        controlSizer->Add(fourDeckShipImg, 0, wxALL, 5);
        BindShipEvents(fourDeckShipImg, 4);
        fourDeckShipText = new wxStaticText(this, wxID_ANY, wxString::Format("x%d", remainingFourDeck));
        controlSizer->Add(fourDeckShipText, 0, wxALL, 5);
    } catch (const std::runtime_error& e) {
        wxLogError("%s", e.what());
    }
}

void MyFrame::OnGridCellDrop(int shipSize, int x, int y) {
    if (!game_ || !game_->canPlaceShip(x, y, shipSize, shipOrientations[shipSize])) {
        wxLogMessage("Ошибка: нельзя размещать корабли так близко друг к другу!");
        return;
    }

    game_->placeShip(x, y, shipSize, shipOrientations[shipSize]);

    if (shipOrientations[shipSize]) {
        // Вертикальная ориентация
        for (int i = 0; i < shipSize; ++i) {
            if ((y + i) < gameGrid->GetNumberRows()) {
                gameGrid->SetCellValue(y + i, x, "X");
                gameGrid->SetCellBackgroundColour(y + i, x, *wxLIGHT_GREY);
            }
        }
    } else {
        // Горизонтальная ориентация
        for (int i = 0; i < shipSize; ++i) {
            if ((x + i) < gameGrid->GetNumberCols()) {
                gameGrid->SetCellValue(y, x + i, "X");
                gameGrid->SetCellBackgroundColour(y, x + i, *wxLIGHT_GREY);
            }
        }
    }

    // Обновляем количество оставшихся кораблей только после успешного размещения
    switch (shipSize) {
        case 1:
            remainingOneDeck--;
            break;
        case 2:
            remainingTwoDeck--;
            break;
        case 3:
            remainingThreeDeck--;
            break;
        case 4:
            remainingFourDeck--;
            break;
        default:
            break;
    }

    UpdateShipCounters();
    gameGrid->ForceRefresh();
}

void MyFrame::UpdateShipCounters() {
    if (oneDeckShipText) oneDeckShipText->SetLabel(wxString::Format("x%d", remainingOneDeck));
    if (twoDeckShipText) twoDeckShipText->SetLabel(wxString::Format("x%d", remainingTwoDeck));
    if (threeDeckShipText) threeDeckShipText->SetLabel(wxString::Format("x%d", remainingThreeDeck));
    if (fourDeckShipText) fourDeckShipText->SetLabel(wxString::Format("x%d", remainingFourDeck));
}

void MyFrame::ClearGridColors() {
    for (int row = 0; row < gameGrid->GetNumberRows(); ++row) {
        for (int col = 0; col < gameGrid->GetNumberCols(); ++col) {
            gameGrid->SetCellBackgroundColour(row, col, *wxWHITE);
        }
    }
}

void MyFrame::BindShipEvents(wxStaticBitmap* shipImg, int shipSize) {
    if (!shipImg) {
        wxLogError("Ошибка: изображение корабля не инициализировано.");
        return;
    }
    shipImg->Bind(wxEVT_LEFT_DOWN, [this, shipImg, shipSize](wxMouseEvent& event) {
        if (game_ && getRemainingShips(shipSize) <= 0) {
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

void MyFrame::ToggleShipOrientation(wxStaticBitmap* shipImg, int shipSize) {
    if (!shipImg) {
        wxLogError("Ошибка: изображение корабля не инициализировано.");
        return;
    }

    // Переключаем состояние ориентации
    shipOrientations[shipSize] = !shipOrientations[shipSize];

    // Проверяем текущую ориентацию и соответствующе вращаем изображение
    wxImage image = shipImg->GetBitmap().ConvertToImage();

    if (shipOrientations[shipSize]) {
        // Устанавливаем вертикальную ориентацию
        if (image.GetWidth() > image.GetHeight()) {
            image = image.Rotate90(true); // Вращаем на 90 градусов против часовой стрелки
        }
    } else {
        // Устанавливаем горизонтальную ориентацию
        if (image.GetHeight() > image.GetWidth()) {
            image = image.Rotate90(true); // Вращаем на 90 градусов против часовой стрелки
        }
    }

    shipImg->SetBitmap(wxBitmap(image));
    wxLogMessage("Изменена ориентация корабля. Теперь он %s", shipOrientations[shipSize] ? "вертикальный" : "горизонтальный");
}

int MyFrame::getRemainingShips(int shipSize) {
    switch (shipSize) {
        case 1:
            return remainingOneDeck;
        case 2:
            return remainingTwoDeck;
        case 3:
            return remainingThreeDeck;
        case 4:
            return remainingFourDeck;
        default:
            return 0;
    }
}
