#include "font.h"

Font::Font()
{
  font_color_ = { 255, 255, 255 };
  text_rect_ = { 0, 0, 0, 0 };
  text_ = "";
}

Font::~Font()
{
  TTF_CloseFont(font_);
  SDL_DestroyTexture(texture_);
  SDL_FreeSurface(surface_);
  TTF_Quit();
}

void Font::init(const char* font_path, int32_t font_size, SDL_Renderer* render, SDL_Color color)
{
  render_ = render;
  TTF_Init();
  font_ = TTF_OpenFont(font_path, font_size);
  font_color_ = color;
  surface_ = TTF_RenderText_Solid(font_, text_.c_str(), font_color_);
  texture_ = SDL_CreateTextureFromSurface(render_, surface_);
  int32_t w, h;
  SDL_QueryTexture(texture_, NULL, NULL, &w, &h);
  text_rect_.w = w;
  text_rect_.h = h;
}

void Font::setTextPosition(int32_t x, int32_t y)
{
  text_rect_.x = x;
  text_rect_.y = y;
}

void Font::setFontColor(SDL_Color color)
{
  font_color_ = color;
}

void Font::updateScoreUI(int32_t new_score)
{
  text_.erase(text_.begin() + 12, text_.end());
  SDL_DestroyTexture(texture_);
  SDL_FreeSurface(surface_);
  text_ += std::to_string(new_score);
  surface_ = TTF_RenderText_Solid(font_, text_.c_str(), font_color_);
  texture_ = SDL_CreateTextureFromSurface(render_, surface_);
}

void Font::setText(const char* text)
{
  text_ = text;
}

void Font::draw()
{
  SDL_RenderCopy(render_, texture_, NULL, &text_rect_);
  //SDL_RenderPresent(render_);
}
