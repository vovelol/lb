// File: src/settings_dialog.cpp
#include "settings_dialog.h"

SettingsDialog::SettingsDialog(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, "Настройки Игры", wxDefaultPosition, wxSize(300, 300)) {

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Размер сетки
    wxBoxSizer* gridSizer = new wxBoxSizer(wxHORIZONTAL);
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Размер сетки:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    gridSizeCtrl = new wxSpinCtrl(this, wxID_ANY);
    gridSizeCtrl->SetRange(5, 20);
    gridSizeCtrl->SetValue(10);
    gridSizer->Add(gridSizeCtrl, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(gridSizer, 0, wxEXPAND);

    // Количество кораблей
    auto addShipSizer = [&](const wxString& label, wxSpinCtrl*& ctrl, int min, int max, int value) {
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new wxStaticText(this, wxID_ANY, label), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        ctrl = new wxSpinCtrl(this, wxID_ANY);
        ctrl->SetRange(min, max);
        ctrl->SetValue(value);
        sizer->Add(ctrl, 1, wxALL | wxEXPAND, 5);
        mainSizer->Add(sizer, 0, wxEXPAND);
    };

    wxSpinCtrl* twoDeckCtrlDummy;
    addShipSizer("Одноклеточные:", oneDeckCtrl, 0, 5, 4);
    addShipSizer("Двухклеточные:", twoDeckCtrl, 0, 5, 3);
    addShipSizer("Трехклеточные:", threeDeckCtrl, 0, 5, 2);
    addShipSizer("Четырехклеточные:", fourDeckCtrl, 0, 5, 1);

    // Кнопки OK и Cancel
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okButton = new wxButton(this, wxID_OK, "OK");
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
    buttonSizer->Add(okButton, 1, wxALL | wxEXPAND, 5);
    buttonSizer->Add(cancelButton, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(buttonSizer, 0, wxEXPAND);

    SetSizerAndFit(mainSizer);

    Bind(wxEVT_BUTTON, &SettingsDialog::OnOK, this, wxID_OK);
    Bind(wxEVT_BUTTON, &SettingsDialog::OnCancel, this, wxID_CANCEL);
}

int SettingsDialog::getGridSize() const {
    return gridSizeCtrl->GetValue();
}

int SettingsDialog::getOneDeckShips() const {
    return oneDeckCtrl->GetValue();
}

int SettingsDialog::getTwoDeckShips() const {
    return twoDeckCtrl->GetValue();
}

int SettingsDialog::getThreeDeckShips() const {
    return threeDeckCtrl->GetValue();
}

int SettingsDialog::getFourDeckShips() const {
    return fourDeckCtrl->GetValue();
}

void SettingsDialog::OnOK(wxCommandEvent& event) {
    EndModal(wxID_OK);
}

void SettingsDialog::OnCancel(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}
