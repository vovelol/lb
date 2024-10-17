// File: src/app.cpp
#include "app.h"
#include "settings_dialog.h"  // Включаем заголовочный файл диалога настроек
#include "ship_drop_target.h" // Включаем ship_drop_target.h здесь, чтобы избежать циклических зависимостей
#include <wx/log.h>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/dnd.h>           // Добавлено для wxDropSource и wxDragResult
#include <memory>
#include <map>  // Для std::map

enum {
    ID_START_BUTTON = wxID_HIGHEST + 1,
};

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, title, pos, size), shipDropTarget(nullptr),
          remainingOneDeck(0), remainingTwoDeck(0), remainingThreeDeck(0), remainingFourDeck(0) {

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
        shipOrientations_[1] = Orientation::Horizontal;
        shipOrientations_[2] = Orientation::Horizontal;
        shipOrientations_[3] = Orientation::Horizontal;
        shipOrientations_[4] = Orientation::Horizontal;

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
    if (game_) {
        AttackOutcome outcome = game_->attackShip(col, row);
        switch (outcome.result) {
            case AttackResult::Hit: {
                // Получаем текущий HP клетки
                int hp = game_->getCellHP(col, row);

                if (hp == 1) {
                    // Поврежденный сегмент
                    wxLogMessage("Поврежденный сегмент корабля!");
                    gameGrid->SetCellValue(row, col, "D"); // D - Damaged
                    gameGrid->SetCellBackgroundColour(row, col, *wxYELLOW);
                } else if (hp == 0) {
                    // Уничтоженный сегмент
                    wxLogMessage("Сегмент корабля уничтожен!");
                    gameGrid->SetCellValue(row, col, "X"); // X - Destroyed
                    gameGrid->SetCellBackgroundColour(row, col, *wxRED);
                } else {
                    // Целый сегмент (на случай, если еще не было попаданий)
                    gameGrid->SetCellValue(row, col, "S"); // S - Ship
                    gameGrid->SetCellBackgroundColour(row, col, *wxLIGHT_GREY);
                }
                break;
            }
            case AttackResult::Miss: {
                wxLogMessage("Мимо!");
                gameGrid->SetCellValue(row, col, "O"); // O - Miss
                gameGrid->SetCellBackgroundColour(row, col, *wxBLUE);
                break;
            }
            case AttackResult::AlreadyHitShip: {
                wxLogMessage("Предупреждение: Эта клетка уже была атакована (уничтоженный сегмент корабля).");
                wxMessageBox("Эта клетка уже была атакована (уничтоженный сегмент корабля).", "Предупреждение", wxOK | wxICON_WARNING);
                break;
            }
            case AttackResult::AlreadyMissed: {
                wxLogMessage("Предупреждение: Эта клетка уже была атакована (промах).");
                wxMessageBox("Эта клетка уже была атакована (промах).", "Предупреждение", wxOK | wxICON_WARNING);
                break;
            }
            default: {
                // Неизвестный результат
                wxLogMessage("Неизвестный результат атаки.");
                break;
            }
        }

        // Обработка автоматически помеченных клеток как "мимо"
        for (const auto& coord : outcome.autoMissedCells) {
            gameGrid->SetCellValue(coord.y, coord.x, "O"); // O - Miss
            gameGrid->SetCellBackgroundColour(coord.y, coord.x, *wxBLUE);
            wxLogMessage("Автоматически помечена клетка (%d, %d) как промах.", coord.x, coord.y);
        }
    } else {
        wxLogMessage("Ошибка: Игра не инициализирована.");
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

    // Инициализируем все клетки как пустые
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            gameGrid->SetCellValue(row, col, ".");
        }
    }
}

void MyFrame::AddShipImages() {
    if (!controlSizer) {
        controlSizer = new wxBoxSizer(wxVERTICAL);
        mainSizer->Add(controlSizer, 0, wxEXPAND | wxALL, 10);
    }

    auto addShip = [&](const std::string& imagePath, int shipSize, int& remainingShips, wxStaticBitmap*& shipImg, wxStaticText*& shipText) {
        shipImg = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxString::FromUTF8(imagePath), wxBITMAP_TYPE_ANY));
        if (!shipImg->GetBitmap().IsOk()) {
            throw std::runtime_error("Не удалось создать bitmap для корабля размером " + std::to_string(shipSize));
        }
        controlSizer->Add(shipImg, 0, wxALL, 5);
        BindShipEvents(shipImg, shipSize);
        shipText = new wxStaticText(this, wxID_ANY, wxString::Format("x%d", remainingShips));
        controlSizer->Add(shipText, 0, wxALL, 5);
    };

    try {
        addShip("images/one_deck.jpg", 1, remainingOneDeck, oneDeckShipImg, oneDeckShipText);
        addShip("images/two_deck.jpg", 2, remainingTwoDeck, twoDeckShipImg, twoDeckShipText);
        addShip("images/three_deck.jpg", 3, remainingThreeDeck, threeDeckShipImg, threeDeckShipText);
        addShip("images/four_deck.jpg", 4, remainingFourDeck, fourDeckShipImg, fourDeckShipText);
    } catch (const std::runtime_error& e) {
        wxLogError("%s", e.what());
    }
}

void MyFrame::OnGridCellDrop(int shipSize, int x, int y) {
    // Получаем ориентацию корабля
    Orientation orientation = shipOrientations_.at(shipSize);

    if (!game_ || !game_->canPlaceShip(x, y, shipSize, orientation)) {
        wxLogMessage("Ошибка: нельзя размещать корабли так близко друг к другу!");
        wxMessageBox("Невозможно разместить корабль в данной позиции.", "Ошибка", wxOK | wxICON_ERROR);
        return;
    }

    game_->placeShip(x, y, shipSize, orientation);

    if (orientation == Orientation::Vertical) {
        // Вертикальная ориентация
        for (int i = 0; i < shipSize; ++i) {
            if ((y + i) < gameGrid->GetNumberRows()) {
                gameGrid->SetCellValue(y + i, x, "X");
                gameGrid->SetCellBackgroundColour(y + i, x, *wxLIGHT_GREY);
            }
        }
    } else { // Horizontal
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
            if (remainingOneDeck > 0) --remainingOneDeck;
            break;
        case 2:
            if (remainingTwoDeck > 0) --remainingTwoDeck;
            break;
        case 3:
            if (remainingThreeDeck > 0) --remainingThreeDeck;
            break;
        case 4:
            if (remainingFourDeck > 0) --remainingFourDeck;
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
            gameGrid->SetCellValue(row, col, ".");
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
            wxMessageBox("Нельзя разместить больше кораблей этого типа.", "Ошибка", wxOK | wxICON_ERROR);
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
    Orientation currentOrientation = shipOrientations_.at(shipSize);
    Orientation newOrientation = (currentOrientation == Orientation::Vertical) ? Orientation::Horizontal : Orientation::Vertical;
    shipOrientations_[shipSize] = newOrientation;

    // Вращаем изображение в зависимости от ориентации
    wxImage image = shipImg->GetBitmap().ConvertToImage();

    if (newOrientation == Orientation::Vertical) {
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
    wxLogMessage("Изменена ориентация корабля. Теперь он %s",
                 (newOrientation == Orientation::Vertical) ? "вертикальный" : "горизонтальный");
}

int MyFrame::getRemainingShips(int shipSize) const {
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

int MyFrame::getCellHP(int x, int y) const {
    if (!game_) return 0;
    return game_->getCellHP(x, y);
}
