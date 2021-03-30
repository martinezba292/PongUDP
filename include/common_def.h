#ifndef _COMMON_DEF_
#define _COMMON_DEF_ 1
#include "SDL.h"
#include "player.h"
#include <ws2tcpip.h>
#include <array>
#include <string>


const int kMax_width = 800;
const int kMax_height = 600;

struct ClientPackage {
  Player playerData_;
  bool close_request;
};

struct ServerPackage {
  std::array<Player,2> playerData_;
  bool close_request;
  //std::string msg;
  char msg[255];
};




#endif // _COMMON_DEF_ 1
