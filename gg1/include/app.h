#ifndef APP_H
#define APP_H

#include "Game.h"
#include "settings_dialog.h"
#include "ship_drop_target.h"
#include <wx/wx.h>
#include <wx/grid.h>
#include <memory>
#include <map> // Используется для хранения ориентаций кораблей

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

    int getRemainingShips(int shipSize);
    void AddShipImage(wxStaticBitmap*& shipImg, wxStaticText*& shipText, const std::string& imagePath, int shipSize, int remainingShips);

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

    bool isVertical = true;
    ShipDropTarget* shipDropTarget;

    int gridSize;
    int oneDeck, twoDeck, threeDeck, fourDeck;

    int remainingOneDeck;
    int remainingTwoDeck;
    int remainingThreeDeck;
    int remainingFourDeck;

    std::unique_ptr<Game> game_;
    std::map<int, bool> shipOrientations; // Хранение ориентаций кораблей: горизонтально или вертикально
};

#endif // APP_H
