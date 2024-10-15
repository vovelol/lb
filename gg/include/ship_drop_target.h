#ifndef SHIP_DROP_TARGET_H
#define SHIP_DROP_TARGET_H

#include <wx/dnd.h>
#include <wx/wx.h>

class MyFrame;

class ShipDropTarget : public wxTextDropTarget {
public:
    ShipDropTarget(MyFrame* frame);
    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data) override;

private:
    MyFrame* frame_;
};

#endif // SHIP_DROP_TARGET_H
