#include "main.h"
#include "app.h"
#include <wx/intl.h>
#include <locale>

// Определение класса MyApp, наследующего wxApp
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Настройка локали для поддержки UTF-8 и русской локали
    try {
        std::locale::global(std::locale(""));
        setlocale(LC_ALL, "");
    } catch (const std::exception& e) {
        wxLogError("Ошибка установки локали: %s", e.what());
        return false;
    }

    wxLocale locale;
    if (!locale.Init(wxLANGUAGE_RUSSIAN)) {
        wxLogError("Не удалось инициализировать локаль для русского языка.");
        return false;
    }
    locale.AddCatalogLookupPathPrefix(".");
    locale.AddCatalog("wxstd");  // Подключение стандартного каталога переводов
    locale.AddCatalog("your_app_name");  // Подключение пользовательского каталога переводов, если есть

    // Создание основного окна
    MyFrame *frame = new MyFrame("Морской бой", wxPoint(50, 50), wxSize(800, 600));
    frame->Show(true);
    return true;
}
