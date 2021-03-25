// PongClient.cpp

#include <stdio.h>
#include "window.h"
#include "common_def.h"

int32_t EstablishConection(SOCKET* sock, sockaddr_in* ip) {
  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 0), &wsa);
  *sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  ip->sin_family = AF_INET;
  InetPton(AF_INET, TEXT("127.0.0.1"), &ip->sin_addr.s_addr);
  ip->sin_port = htons(9999);
  char buffer[254];
  int ip_size = sizeof(*ip);
  strcpy_s(buffer, "New client connected");
  if (sendto(*sock, buffer, sizeof(buffer), 0, (SOCKADDR*)ip, sizeof(*ip)) == SOCKET_ERROR) {
    printf("ERROR SENDING DATA");
    return -1;
  }
  uint32_t player_tag;
  recvfrom(*sock, (char*)&player_tag, sizeof(player_tag), 0, (SOCKADDR*)ip, &ip_size);
  printf("You're the player %d\n", player_tag + 1);
  memset(buffer, 0, sizeof(buffer));
  recvfrom(*sock, buffer, sizeof(buffer), 0, (SOCKADDR*)ip, &ip_size);
  printf("%s", buffer);

  return player_tag;
}

bool CloseRequest(SDL_Event* event) {
  return (event->type == SDL_QUIT) || 
         (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE);
}

void SendoToServer(const SOCKET& sock, 
                   const ClientPackage& packet,
                   const sockaddr_in& ip) {

  int ip_size = sizeof(ip);
  sendto(sock, (char*)&packet, sizeof(ClientPackage), 0, (SOCKADDR*)&ip, ip_size);
}

ServerPackage ReceiveFromServer(const SOCKET& sock, const sockaddr_in& ip) {
  int ip_size = sizeof(ip);
  ServerPackage packet;
  recvfrom(sock, (char*)&packet, sizeof(ServerPackage), 0, (SOCKADDR*)&ip, &ip_size);

  return packet;
}

int main(int argc, char** argv)
{
  SOCKET sock;
  struct sockaddr_in ip;
  ClientPackage player;
  int ip_size = sizeof(ip);
  player.playerData_.init(EstablishConection(&sock, &ip));
  bool close_request = false;
  Window::getInternalData().init(kMax_width, kMax_height, 0, 0, 0);
  ServerPackage game_info;
  game_info = ReceiveFromServer(sock, ip);
  int32_t client_tag = player.playerData_.getTag();
  player.playerData_ = game_info.playerData_[client_tag];
  while (!close_request) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      close_request = CloseRequest(&event);

      player.playerData_.playerController(event, SDLK_w, SDLK_s);
    }

    SendoToServer(sock, player, ip);
    game_info = ReceiveFromServer(sock, ip);
    player.playerData_.setPosition(game_info.playerData_[client_tag]);

    Window::getInternalData().clearWindow();
    game_info.playerData_[0].draw(Window::getInternalData().render_);
    game_info.playerData_[1].draw(Window::getInternalData().render_);
    Window::getInternalData().renderDraw();
  }
  return 0;
}