#ifndef __PLAYER_H__
#define __PLAYER_H__ 1

#include <stdint.h>
#include "SDL.h"

class Font;
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
  int32_t getScore();
  void updateScore();
  void synchronizeUI(Font& font);
  SDL_Rect getRect();
  void movePosition();
  void setLimits(int height);
  int32_t getWidth();
  int32_t getHeight();


private:
  int32_t tag_;
  int32_t direction_;
  int32_t speed_;
  int32_t score_;
  bool score_outdated_;
  SDL_Rect playerRect_;
};

#endif // !__PLAYER_H__ 1

