// File: include/app.h
#ifndef APP_H
#define APP_H

#include "common.h"            // Для Orientation
#include "Game.h"
#include "settings_dialog.h"
#include <wx/wx.h>
#include <wx/grid.h>
#include <memory>
#include <map>

// Предварительное объявление класса ShipDropTarget
class ShipDropTarget;

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MyFrame();

    wxGrid* getGameGrid() const;
    void StartGame();
    void OnStartButtonClicked(wxCommandEvent& event);
    void OnGridAttackClick(wxGridEvent& gridEvent);
    void OnGridCellDrop(int shipSize, int x, int y); // Объявление функции
    void UpdateShipCounters();
    void InitializeGrid(int size);
    void AddShipImages();
    void BindShipEvents(wxStaticBitmap* shipImg, int shipSize);
    void ToggleShipOrientation(wxStaticBitmap* shipImg, int shipSize);
    void ClearGridColors();

    int getRemainingShips(int shipSize) const;
    void AddShipImage(wxStaticBitmap*& shipImg, wxStaticText*& shipText, const std::string& imagePath, int shipSize, int remainingShips);

    // Добавляем объявление метода для получения HP клетки
    int getCellHP(int x, int y) const; // Метод для получения HP клетки

private:
    wxBoxSizer* mainSizer;
    wxBoxSizer* controlSizer;
    wxButton* startButton;
    wxGrid* gameGrid;

    wxStaticBitmap* oneDeckShipImg;
    wxStaticBitmap* twoDeckShipImg;
    wxStaticBitmap* threeDeckShipImg;
    wxStaticBitmap* fourDeckShipImg;

    wxStaticText* oneDeckShipText;
    wxStaticText* twoDeckShipText;
    wxStaticText* threeDeckShipText;
    wxStaticText* fourDeckShipText;

    ShipDropTarget* shipDropTarget; // Указатель на ShipDropTarget

    int gridSize;
    int oneDeck, twoDeck, threeDeck, fourDeck;

    int remainingOneDeck;
    int remainingTwoDeck;
    int remainingThreeDeck;
    int remainingFourDeck;

    std::unique_ptr<Game> game_;
    std::map<int, Orientation> shipOrientations_; // Хранение ориентаций кораблей: горизонтально или вертикально
};

#endif // APP_H
