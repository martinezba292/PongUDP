#ifndef __PLAYER_H__
#define __PLAYER_H__ 1

#include <stdint.h>
#include "SDL.h"


class Player {
public:
  Player();
  ~Player(){}
  Player(const Player&);
  void init(uint32_t player_tag);
  void setSpeed(float);
  int32_t getTag();
  void setPosition(int x, int y);
  void setPosition(Player other);
  void playerController(SDL_Event event, SDL_Keycode up, SDL_Keycode down);
  void movePosition();
  void setLimits(int height);
  void draw(SDL_Renderer* rend);
  int32_t getWidth();
  int32_t getHeight();


private:
  int32_t tag_;
  int32_t direction_;
  int32_t speed_;
  float score_;
  SDL_Rect playerRect_;



};

#endif // !__PLAYER_H__ 1

