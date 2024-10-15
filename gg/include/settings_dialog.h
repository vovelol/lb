#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>

class SettingsDialog : public wxDialog {
public:
    SettingsDialog(wxWindow* parent);

    int GetSelectedGridSize() const;
    wxArrayInt GetSelectedShips() const;

private:
    wxSpinCtrl* gridSizeCtrl;
    wxSpinCtrl* oneDeckCtrl;
    wxSpinCtrl* twoDeckCtrl;
    wxSpinCtrl* threeDeckCtrl;
    wxSpinCtrl* fourDeckCtrl;
};

#endif  // SETTINGS_DIALOG_H
