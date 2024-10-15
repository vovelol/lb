#include "app.h"
#include <wx/intl.h>
#include <locale>

// Макрос для запуска приложения wxWidgets
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Настройка локали для поддержки UTF-8 и русской локали
    std::locale::global(std::locale("ru_RU.UTF-8"));
    setlocale(LC_ALL, "ru_RU.UTF-8");

    wxLocale locale;
    locale.Init(wxLANGUAGE_RUSSIAN);
    locale.AddCatalogLookupPathPrefix(".");
    locale.AddCatalog("wxstd");  // Подключение стандартного каталога переводов
    locale.AddCatalog("your_app_name");  // Подключение пользовательского каталога переводов, если есть

    // Создание основного окна
    MyFrame *frame = new MyFrame("Морской бой", wxPoint(50, 50), wxSize(800, 600));
    frame->Show(true);
    return true;
}
