#ifndef _COMMON_DEF_
#define _COMMON_DEF_ 1
#include "SDL.h"
#include "player.h"
#include <ws2tcpip.h>
#include <array>


const int kMax_width = 800;
const int kMax_height = 600;

struct ClientPackage {
  Player playerData_;
};

struct ServerPackage {
  std::array<Player,2> playerData_;
};




#endif // _COMMON_DEF_ 1
