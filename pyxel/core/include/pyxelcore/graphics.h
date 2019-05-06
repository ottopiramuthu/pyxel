#ifndef PYXELCORE_GRAPHICS_H_
#define PYXELCORE_GRAPHICS_H_

#include "pyxelcore/rectangle.h"

namespace pyxelcore {

class Image;
class Tilemap;

class Graphics {
 public:
  Graphics(int32_t width, int32_t height);
  ~Graphics();

  const Rectangle& ClipArea() const { return clip_area_; }
  const int32_t* PaletteTable() const { return palette_table_; }
  const int32_t* ScreenData() const { return screen_data_; }

  Image* GetImageBank(int32_t image_index, bool system = false) const;
  Tilemap* GetTilemapBank(int32_t tilemap_index) const;

  void ResetClipArea();
  void SetClipArea(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

  void ResetPalette();
  void SetPalette(int32_t src_color, int32_t dst_color);

  void ClearScreen(int32_t color);
  void DrawPoint(int32_t x, int32_t y, int32_t color);
  void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t color);
  void DrawRectangle(int32_t x1,
                     int32_t y1,
                     int32_t x2,
                     int32_t y2,
                     int32_t color);
  void DrawRectangleBorder(int32_t x1,
                           int32_t y1,
                           int32_t x2,
                           int32_t y2,
                           int32_t color);
  void DrawCircle(int32_t x, int32_t y, int32_t radius, int32_t color);
  void DrawCircleBorder(int32_t x, int32_t y, int32_t radius, int32_t color);
  void DrawImage(int32_t x,
                 int32_t y,
                 int32_t image_index,
                 int32_t u,
                 int32_t v,
                 int32_t width,
                 int32_t height,
                 int32_t color_key = -1);
  void DrawTilemap(int32_t x,
                   int32_t y,
                   int32_t tilemap_index,
                   int32_t u,
                   int32_t v,
                   int32_t width,
                   int32_t height,
                   int32_t color_key = -1);
  void DrawText(int32_t x, int32_t y, const char* text, int32_t color);

 private:
  Image* screen_image_;
  int32_t screen_width_;
  int32_t screen_height_;
  int32_t* screen_data_;
  Image** image_bank_;
  Tilemap** tilemap_bank_;
  Rectangle clip_area_;
  int32_t palette_table_[COLOR_COUNT];

  void SetupMouseCursor();
  void SetupFont();
  int32_t GetDrawColor(int32_t color) const;
  void SetPixel(int32_t x, int32_t y, int32_t color);
};

inline Image* Graphics::GetImageBank(int32_t image_index, bool system) const {
  if (image_index < 0 || image_index >= IMAGE_BANK_COUNT) {
    PRINT_ERROR("invalid image index");
    image_index = 0;
  }

  if (image_index == IMAGE_BANK_FOR_SYSTEM && !system) {
    PRINT_ERROR("access to image bank for system");
  }

  return image_bank_[image_index];
}

inline Tilemap* Graphics::GetTilemapBank(int32_t tilemap_index) const {
  if (tilemap_index < 0 || tilemap_index >= TILEMAP_BANK_COUNT) {
    PRINT_ERROR("invalid tilemap index");
    tilemap_index = 0;
  }

  return tilemap_bank_[tilemap_index];
}

inline int32_t Graphics::GetDrawColor(int32_t color) const {
  if (color < 0 || color >= COLOR_COUNT) {
    PRINT_ERROR("invalid color");
    return 0;
  }

  return palette_table_[color];
}

inline void Graphics::SetPixel(int32_t x, int32_t y, int32_t draw_color) {
  if (clip_area_.Includes(x, y)) {
    screen_data_[screen_width_ * y + x] = draw_color;
  }
}

}  // namespace pyxelcore

#endif  // PYXELCORE_GRAPHICS_H_