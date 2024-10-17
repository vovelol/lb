#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>  // Добавляем этот заголовочный файл для использования wxSpinCtrl

class SettingsDialog : public wxDialog {
public:
    SettingsDialog(wxWindow* parent);

    int getGridSize() const;
    int getOneDeckShips() const;
    int getTwoDeckShips() const;
    int getThreeDeckShips() const;
    int getFourDeckShips() const;

private:
    wxSpinCtrl* gridSizeInput;
    wxSpinCtrl* oneDeckInput;
    wxSpinCtrl* twoDeckInput;
    wxSpinCtrl* threeDeckInput;
    wxSpinCtrl* fourDeckInput;

    void OnOk(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif // SETTINGS_DIALOG_H
