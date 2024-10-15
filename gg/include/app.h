#ifndef APP_H
#define APP_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <vector>
#include <memory>  // Для использования unique_ptr
#include "Game.h"
#include "ShipSegment.h"

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

    void setGame(std::unique_ptr<Game> game);
    void InitializeGrid(int rows, int cols, int oneDeck, int twoDeck, int threeDeck, int fourDeck);
    
    wxGrid* getGameGrid() const;

    void OnGridCellDrop(int shipSize, int x, int y);
    void OnStartGame(wxCommandEvent& event);

private:
    std::vector<std::vector<char>> gridState;
    std::vector<std::vector<ShipSegment>> shipInfo;
    wxGrid* gameGrid = nullptr;

    wxBoxSizer* mainSizer;
    wxBoxSizer* rightSizer;

    wxStaticBitmap* oneDeckShipImg;
    wxStaticBitmap* twoDeckShipImg;
    wxStaticBitmap* threeDeckShipImg;
    wxStaticBitmap* fourDeckShipImg;

    wxStaticText* oneDeckShipText;
    wxStaticText* twoDeckShipText;
    wxStaticText* threeDeckShipText;
    wxStaticText* fourDeckShipText;

    int selectedShipSize = -1;
    bool isVertical = false;
    bool isGameStarted = false;

    std::unique_ptr<Game> game_;  // Изменено с обычного объекта на std::unique_ptr

    ShipDropTarget* shipDropTarget = nullptr;  // Объект ShipDropTarget

    void AddShipImages();
    void BindShipEvents(wxStaticBitmap* shipImg, int shipSize);
    void OnGridCellClick(wxGridEvent& gridEvent);
    void OnGridAttackClick(wxGridEvent& gridEvent);
    void ShowSettingsDialog();
    void UpdateShipCounters();
    void DecreaseShipCount(int shipSize);
    void ToggleShipOrientation(wxStaticBitmap* shipImg, int shipSize);
    
    wxDECLARE_EVENT_TABLE();
};

#endif  // APP_H
