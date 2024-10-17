#ifndef BITMAP_RENDERER_H
#define BITMAP_RENDERER_H

#include <wx/dc.h>
#include <string>

class BitmapRenderer {
public:
    BitmapRenderer(const std::string& imagePath);

    void render(wxDC& dc, int x, int y);

private:
    wxBitmap bitmap_;
};

#endif  // BITMAP_RENDERER_H