#ifndef __PONG_FONT_H__
#define __PONG_FONT_H__ 1

#include <string>
#include "SDL_ttf.h"
#include "window.h"

class Font {

public:
  Font();
  ~Font();
  void init(const char* font_path, int32_t font_size, SDL_Renderer* render, SDL_Color color = { 255, 255, 255 });
  void setTextPosition(int32_t x, int32_t y);
  void setFontColor(SDL_Color color);
  void updateScoreUI(int32_t new_score);
  void setText(const char* text);
  void draw();

private:
  std::string text_;
  TTF_Font* font_;
  SDL_Color font_color_;
  SDL_Texture* texture_;
  SDL_Surface* surface_;
  SDL_Rect text_rect_;
  SDL_Renderer* render_;
};

#endif // !__PONG_FONT_H__ 1

