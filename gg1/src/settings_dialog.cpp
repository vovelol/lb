#include "settings_dialog.h"
#include <wx/spinctrl.h>  // Обязательно включаем этот заголовок

enum {
    ID_OK = wxID_HIGHEST + 1
};

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
    EVT_BUTTON(ID_OK, SettingsDialog::OnOk)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Настройки игры", wxDefaultPosition, wxSize(300, 400)) {

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Размер поля (квадратное):"), 0, wxALL, 5);
    gridSizeInput = new wxSpinCtrl(this, wxID_ANY);
    gridSizeInput->SetRange(5, 20);
    gridSizeInput->SetValue(10);
    mainSizer->Add(gridSizeInput, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Однопалубные корабли:"), 0, wxALL, 5);
    oneDeckInput = new wxSpinCtrl(this, wxID_ANY);
    oneDeckInput->SetRange(0, 10);
    oneDeckInput->SetValue(4);
    mainSizer->Add(oneDeckInput, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Двухпалубные корабли:"), 0, wxALL, 5);
    twoDeckInput = new wxSpinCtrl(this, wxID_ANY);
    twoDeckInput->SetRange(0, 10);
    twoDeckInput->SetValue(2);
    mainSizer->Add(twoDeckInput, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Трехпалубные корабли:"), 0, wxALL, 5);
    threeDeckInput = new wxSpinCtrl(this, wxID_ANY);
    threeDeckInput->SetRange(0, 10);
    threeDeckInput->SetValue(2);
    mainSizer->Add(threeDeckInput, 0, wxEXPAND | wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Четырехпалубные корабли:"), 0, wxALL, 5);
    fourDeckInput = new wxSpinCtrl(this, wxID_ANY);
    fourDeckInput->SetRange(0, 10);
    fourDeckInput->SetValue(1);
    mainSizer->Add(fourDeckInput, 0, wxEXPAND | wxALL, 5);

    wxButton* okButton = new wxButton(this, ID_OK, "OK");
    mainSizer->Add(okButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
    Layout();
}

int SettingsDialog::getGridSize() const {
    return gridSizeInput->GetValue();
}

int SettingsDialog::getOneDeckShips() const {
    return oneDeckInput->GetValue();
}

int SettingsDialog::getTwoDeckShips() const {
    return twoDeckInput->GetValue();
}

int SettingsDialog::getThreeDeckShips() const {
    return threeDeckInput->GetValue();
}

int SettingsDialog::getFourDeckShips() const {
    return fourDeckInput->GetValue();
}

void SettingsDialog::OnOk(wxCommandEvent& event) {
    EndModal(wxID_OK);
}
