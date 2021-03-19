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


private:
  int32_t tag_;
  int32_t direction_;
  float speed_;
  float score_;
  SDL_Rect playerRect_;



};

#endif // !__PLAYER_H__ 1

