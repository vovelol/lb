#include "settings_dialog.h"

SettingsDialog::SettingsDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Настройки игры", wxDefaultPosition, wxDefaultSize) {
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Настройка выбора размера поля
    wxStaticText* gridSizeLabel = new wxStaticText(this, wxID_ANY, "Размер поля:");
    gridSizeCtrl = new wxSpinCtrl(this, wxID_ANY);
    gridSizeCtrl->SetRange(5, 20);  // Например, от 5x5 до 20x20
    gridSizeCtrl->SetValue(10);  // Значение по умолчанию

    // Настройка выбора количества кораблей
    wxStaticText* oneDeckLabel = new wxStaticText(this, wxID_ANY, "Однопалубные корабли:");
    oneDeckCtrl = new wxSpinCtrl(this, wxID_ANY);
    oneDeckCtrl->SetRange(0, 4);
    oneDeckCtrl->SetValue(4);

    wxStaticText* twoDeckLabel = new wxStaticText(this, wxID_ANY, "Двухпалубные корабли:");
    twoDeckCtrl = new wxSpinCtrl(this, wxID_ANY);
    twoDeckCtrl->SetRange(0, 3);
    twoDeckCtrl->SetValue(3);

    wxStaticText* threeDeckLabel = new wxStaticText(this, wxID_ANY, "Трехпалубные корабли:");
    threeDeckCtrl = new wxSpinCtrl(this, wxID_ANY);
    threeDeckCtrl->SetRange(0, 2);
    threeDeckCtrl->SetValue(2);

    wxStaticText* fourDeckLabel = new wxStaticText(this, wxID_ANY, "Четырехпалубные корабли:");
    fourDeckCtrl = new wxSpinCtrl(this, wxID_ANY);
    fourDeckCtrl->SetRange(0, 1);
    fourDeckCtrl->SetValue(1);

    // Добавляем все элементы в sizer
    sizer->Add(gridSizeLabel, 0, wxALL, 5);
    sizer->Add(gridSizeCtrl, 0, wxALL, 5);
    sizer->Add(oneDeckLabel, 0, wxALL, 5);
    sizer->Add(oneDeckCtrl, 0, wxALL, 5);
    sizer->Add(twoDeckLabel, 0, wxALL, 5);
    sizer->Add(twoDeckCtrl, 0, wxALL, 5);
    sizer->Add(threeDeckLabel, 0, wxALL, 5);
    sizer->Add(threeDeckCtrl, 0, wxALL, 5);
    sizer->Add(fourDeckLabel, 0, wxALL, 5);
    sizer->Add(fourDeckCtrl, 0, wxALL, 5);

    // Добавляем кнопки OK и Cancel
    sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(sizer);
}

int SettingsDialog::GetSelectedGridSize() const {
    return gridSizeCtrl->GetValue();
}

wxArrayInt SettingsDialog::GetSelectedShips() const {
    wxArrayInt ships;
    ships.Add(oneDeckCtrl->GetValue());
    ships.Add(twoDeckCtrl->GetValue());
    ships.Add(threeDeckCtrl->GetValue());
    ships.Add(fourDeckCtrl->GetValue());
    return ships;
}
