#ifndef _COMMON_DEF_
#define _COMMON_DEF_ 1
#include "SDL.h"
#include "player.h"
#include <ws2tcpip.h>
#include <array>
#include <string>
#include "ball.h"
#include <tchar.h>

const LPCWSTR kServer_IP = _T("127.0.0.1");

const int kMax_width = 800;
const int kMax_height = 600;

enum GameState {
  kGameState_Countdown = 0,
  kGameState_Playing = 1,
};

struct ClientPackage {
  Player playerData_;
  bool close_request;
  GameState client_state;
};

struct ServerPackage {
  std::array<Player,2> playerData_;
  SDL_Rect ball;
  GameState game_state;
  bool close_request;
  char msg[255];
};




#endif // _COMMON_DEF_ 1
