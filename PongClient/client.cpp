// PongClient.cpp

#include <stdio.h>
#include "window.h"
#include "common_def.h"
#include <iostream>
#include "font.h"


int32_t EstablishConection(SOCKET* sock, sockaddr_in* ip) {
  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 0), &wsa);
  *sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  ip->sin_family = AF_INET;
  InetPton(AF_INET, kServer_IP, &ip->sin_addr.s_addr);
  ip->sin_port = htons(8888);
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
  ServerPackage packet{};
  int32_t bytes = recvfrom(sock, (char*)&packet, sizeof(ServerPackage), 0, (SOCKADDR*)&ip, &ip_size);
  if (bytes == SOCKET_ERROR)
  {
    std::cout << "'recvfrom' failed with error " << WSAGetLastError();
    std::cout << std::endl;
  }
  return packet;
}

void Draw(const SDL_Rect& rect, uint8_t rgb[3]) {
  SDL_Renderer* render = Window::getInternalData().render_;
  SDL_SetRenderDrawColor(render, rgb[0], rgb[1], rgb[2], 255);
  SDL_RenderFillRect(render, &rect);
}

ServerPackage CountDown(const SOCKET& sock, const sockaddr_in& ip, ClientPackage* package) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    package->close_request = CloseRequest(&event);
    package->playerData_.playerController(event, SDLK_w, SDLK_s);
  }
  ServerPackage game_info{};
  int32_t ip_size = sizeof(ip);
  SendoToServer(sock, *package, ip);
  game_info = ReceiveFromServer(sock, ip);
  package->client_state = game_info.game_state;
  return game_info;
}

ServerPackage Input(const SOCKET& sock, const sockaddr_in& ip, ClientPackage* package) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    package->close_request = CloseRequest(&event);
    package->playerData_.playerController(event, SDLK_w, SDLK_s);
  }
  SendoToServer(sock, *package, ip);
  ServerPackage game_info{};
  int32_t ip_size = sizeof(ip);
  recvfrom(sock, (char*)&game_info, sizeof(ServerPackage), 0, (SOCKADDR*)&ip, &ip_size);
  return game_info;
}

int main(int argc, char** argv)
{
  SOCKET sock;
  struct sockaddr_in ip;
  ClientPackage player;
  int ip_size = sizeof(ip);
  player.playerData_.init(EstablishConection(&sock, &ip));
  player.client_state = kGameState_Countdown;
  ServerPackage game_info = ReceiveFromServer(sock, ip);
  Window::getInternalData().init(kMax_width, kMax_height, 0, 0, 0);
  int32_t client_tag = player.playerData_.getTag();
  player.playerData_ = game_info.playerData_[client_tag];
  player.close_request = false;
  uint8_t color[3] = { 255, 255, 255 };
  std::string scorestr = "    " + std::to_string(player.playerData_.getScore());
  const char tag[2] = { '1', '2' };
  std::string usernick = "PLAYER ";
  usernick += tag[client_tag];
  usernick += scorestr;
  std::string rivalnick = "PLAYER ";
  rivalnick += tag[client_tag ^ 1];
  rivalnick += scorestr;

  Font ui;
  ui.setText(usernick.c_str());
  ui.init("./../data/font/aesymatt.ttf", 20, Window::getInternalData().render_);
  ui.setTextPosition(600 * client_tag, 0);

  Font ui2;
  ui2.setText(rivalnick.c_str());
  SDL_Color rival_color = { 255, 50, 50 };
  ui2.init("./../data/font/aesymatt.ttf", 20, Window::getInternalData().render_, rival_color);
  int32_t rival_tag = client_tag ^ 1;
  ui2.setTextPosition(600 * rival_tag, 0);

  ServerPackage (*f[2])(const SOCKET& sock, const sockaddr_in& ip, ClientPackage* package);
  f[0] = CountDown;
  f[1] = Input;
  while (!game_info.close_request) {
    game_info = f[player.client_state](sock, ip, &player);

    player.playerData_ = game_info.playerData_[client_tag];
    player.playerData_.synchronizeUI(ui);

    game_info.playerData_[rival_tag].synchronizeUI(ui2);

    Window::getInternalData().clearWindow();
    Draw(game_info.playerData_[client_tag].getRect(), color);
    Draw(game_info.playerData_[rival_tag].getRect(), &rival_color.r);
    Draw(game_info.ball, color);
    ui.draw();
    ui2.draw();
    Window::getInternalData().renderDraw();
  }

  int finish_confirm = 1;
  sendto(sock, (char*)&finish_confirm, sizeof(int), 0, (SOCKADDR*)&ip, ip_size);
  printf(game_info.msg);
  shutdown(sock, SD_BOTH);
  closesocket(sock);
  WSACleanup();
  Window::getInternalData().destroy();

  return 0;
}