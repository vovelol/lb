#ifndef APP_H
#define APP_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <vector>
#include <memory>
#include "Game.h"

// Предварительное объявление класса ShipDropTarget
class ShipDropTarget;

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    ~MyFrame();

    wxGrid* getGameGrid() const;

    void StartGame();

    void OnGridCellDrop(int shipSize, int x, int y);
    void OnStartButtonClicked(wxCommandEvent& event);
    void OnGridAttackClick(wxGridEvent& event);

private:
    void InitializeGrid(int size);
    void BindShipEvents(wxStaticBitmap* shipImg, int shipSize);
    void ToggleShipOrientation(wxStaticBitmap* shipImg, int shipSize);
    void UpdateShipCounters();
    void AddShipImages();

    wxBoxSizer* mainSizer;
    wxBoxSizer* controlSizer;  // Добавляем controlSizer для размещения кораблей и кнопок

    wxGrid* gameGrid = nullptr;

    wxButton* startButton = nullptr;

    wxStaticBitmap* oneDeckShipImg;
    wxStaticBitmap* twoDeckShipImg;
    wxStaticBitmap* threeDeckShipImg;
    wxStaticBitmap* fourDeckShipImg;

    wxStaticText* oneDeckShipText;
    wxStaticText* twoDeckShipText;
    wxStaticText* threeDeckShipText;
    wxStaticText* fourDeckShipText;

    int gridSize;
    int oneDeck;
    int twoDeck;
    int threeDeck;
    int fourDeck;
    bool isVertical = false;

    std::unique_ptr<Game> game_;  
    ShipDropTarget* shipDropTarget = nullptr;

    wxDECLARE_EVENT_TABLE();
};

#endif  // APP_H
