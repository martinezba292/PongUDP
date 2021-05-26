#ifndef __PONGWINDOW_H__
#define __PONGWINDOW_H__ 1

#include <SDL.h>
#include <stdio.h>

class Window {
public:
  SDL_Renderer* render_;
  Window();
  void init();
  void init(int window_width, int window_height,
    Uint8 r = 0, Uint8 g = 0, Uint8 b = 0);
  static Window& getInternalData();
  void destroy();
  void setWindowColor(Uint8 r, Uint8 g, Uint8 b);
  void clearWindow();
  void renderDraw();

private:
  SDL_Color window_color_;
  static Window* internal_data_;
  SDL_Window* window_;
  SDL_GLContext gl_context_;
  SDL_DisplayMode current_;
  ~Window();
  int initVideo();
  int windowCreate(int width, int height);
  int createRenderer();
};

#endif