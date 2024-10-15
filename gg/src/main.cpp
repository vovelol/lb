#include "app.h"
#include <wx/wx.h>
#include <locale.h>  // Для работы с локалью
#include "settings_dialog.h"  // Подключаем для использования диалога настроек

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Устанавливаем русскую локаль
    setlocale(LC_ALL, "ru_RU.UTF-8");

    // Создаем и показываем диалог настроек
    SettingsDialog* settingsDialog = new SettingsDialog(nullptr);
    if (settingsDialog->ShowModal() == wxID_OK) {
        // Извлекаем введенные пользователем значения
        int gridSize = settingsDialog->GetSelectedGridSize();
        wxArrayInt ships = settingsDialog->GetSelectedShips();

        // Создаем основное окно с параметрами от пользователя
        MyFrame* frame = new MyFrame("Морской бой", wxPoint(50, 50), wxSize(800, 600));
        frame->InitializeGrid(gridSize, gridSize, ships[0], ships[1], ships[2], ships[3]);
        frame->Show(true);
    }

    settingsDialog->Destroy();  // Уничтожаем диалог после использования
    return true;
}
