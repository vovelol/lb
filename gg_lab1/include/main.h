#ifndef MAIN_H
#define MAIN_H

#include <wx/wx.h>
#include "app.h"

// Основной класс приложения
class MyApp : public wxApp {
public:
    virtual bool OnInit() override;
};

#endif // MAIN_H
