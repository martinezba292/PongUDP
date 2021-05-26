#ifndef __BALL_H__
#define __BALL_H__ 1

#include "SDL.h"
#include <vector>

struct Vector2D {
  float x;
  float y;
};

class Ball {
public:
  Ball(int32_t speed, int32_t width, int32_t height);
  ~Ball(){}

  SDL_Rect getRect();
  void update(std::vector<SDL_Rect> players);
  void reset();
  void draw(SDL_Renderer* render);

private:
  void collision(const SDL_Rect& player);

  SDL_Rect ball_;
  int32_t speed_;
  Vector2D direction_;
};

#endif
