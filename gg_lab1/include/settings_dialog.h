#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h> // Добавлено для wxSpinCtrl

class SettingsDialog : public wxDialog {
public:
    SettingsDialog(wxWindow* parent);

    int getGridSize() const;
    int getOneDeckShips() const;
    int getTwoDeckShips() const;
    int getThreeDeckShips() const;
    int getFourDeckShips() const;

private:
    wxSpinCtrl* gridSizeCtrl;
    wxSpinCtrl* oneDeckCtrl;
    wxSpinCtrl* twoDeckCtrl;
    wxSpinCtrl* threeDeckCtrl;
    wxSpinCtrl* fourDeckCtrl;

    void OnOK(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
};

#endif // SETTINGS_DIALOG_H
