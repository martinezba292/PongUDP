// PongServer.cpp

#include <stdio.h>
#include "common_def.h"
#include <chrono>

ClientPackage RecieveFromClient(const SOCKET& sock) {
  ClientPackage player_info;
  recvfrom(sock, (char*)&player_info, sizeof(ClientPackage), 0, NULL, NULL);
  return player_info;
}

void Update(ServerPackage* packet, Ball* ball) {
  for (size_t i = 0; i < packet->playerData_.size(); i++) {
    packet->playerData_[i].movePosition();
    packet->playerData_[i].setLimits(kMax_height);
  }
  std::vector<SDL_Rect> players{ packet->playerData_[0].getRect(), packet->playerData_[1].getRect() };
  ball->update(players);
  SDL_Rect ball_pos = ball->getRect();
  float center_x = ball_pos.x + (ball_pos.w >> 1);
  if (center_x <= 0) {
    ball->reset();
    packet->playerData_[1].updateScore();
  }
  else if (center_x >= kMax_width) {
    ball->reset();
    packet->playerData_[0].updateScore();
  }
  packet->ball = ball->getRect();
}

bool CountDown(const SOCKET& sock, const sockaddr_in* ip, ServerPackage* package, std::chrono::steady_clock::time_point start) {
  std::chrono::duration<double> elapsed = elapsed.zero();
  auto end = std::chrono::steady_clock::now();
  elapsed = end - start;
  package->game_state = (GameState)(elapsed.count() >= 3.0f);
  ClientPackage client1 = RecieveFromClient(sock);
  ClientPackage client2 = RecieveFromClient(sock);

  package->playerData_[client1.playerData_.getTag()] = client1.playerData_;
  package->playerData_[client2.playerData_.getTag()] = client2.playerData_;
  static bool close_request[2];
  close_request[0] = client1.close_request;
  close_request[1] = client2.close_request;
  for (size_t i = 0; i < package->playerData_.size(); i++) {
    package->playerData_[i].movePosition();
    package->playerData_[i].setLimits(kMax_height);
  }

  sendto(sock, (char*)package, sizeof(ServerPackage), 0, (SOCKADDR*)&ip[0], sizeof(ip[0]));
  sendto(sock, (char*)package, sizeof(ServerPackage), 0, (SOCKADDR*)&ip[1], sizeof(ip[1]));
  package->game_state = (GameState)(client1.client_state && client2.client_state);

  return package->game_state;
}

bool* GameLoop(const SOCKET& sock, const sockaddr_in* ip, ServerPackage* package, Ball* game_ball) {
  ClientPackage client1 = RecieveFromClient(sock);
  ClientPackage client2 = RecieveFromClient(sock);
  package->playerData_[client1.playerData_.getTag()] = client1.playerData_;
  package->playerData_[client2.playerData_.getTag()] = client2.playerData_;
  static bool close_request[2];
  close_request[0] = client1.close_request;
  close_request[1] = client2.close_request;
  Update(package, game_ball);

  sendto(sock, (char*)package, sizeof(ServerPackage), 0, (SOCKADDR*)&ip[0], sizeof(ip[0]));
  sendto(sock, (char*)package, sizeof(ServerPackage), 0, (SOCKADDR*)&ip[1], sizeof(ip[1]));

  return close_request;
}

int main(int argc, char** argv)
{
  WSADATA wsa;
  SOCKET sock;
  struct sockaddr_in ip, ipc[2];
  srand(time(NULL));
  WSAStartup(MAKEWORD(2, 0), &wsa);
  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  ip.sin_family = AF_INET;
  ip.sin_addr.s_addr = htonl(INADDR_ANY);
  ip.sin_port = htons(8888);
  bind(sock, (SOCKADDR*)&ip, sizeof(ip));
  char new_client[254];
  int ip_size = sizeof(ipc[0]);
  int players_number = 0;
  ServerPackage packet;
  Ball* game_ball = new Ball(7, 20, 20);
  packet.close_request = false;
  packet.game_state = kGameState_Countdown;

  printf("Server online, waiting for players\n");
  while (players_number < 2) {
    memset(new_client, 0, sizeof(new_client));
    recvfrom(sock, new_client, sizeof(new_client), 0,
      (SOCKADDR*)&ipc[players_number], &ip_size);
    packet.playerData_[players_number].init(players_number);
    sendto(sock, (char*)&players_number, sizeof(players_number),
      0, (SOCKADDR*)&ipc[players_number], ip_size);
    ++players_number;
    printf("%s Connected players: %d\n", new_client, players_number);
  }
  
  //WARNS player that the game is on
  printf("Starting game\n");
  memset(new_client, 0, sizeof(new_client));
  strcpy_s(new_client, "Begin!!\n");
  for (int i = 0; i < players_number; ++i) {
    sendto(sock, new_client, sizeof(new_client), 0, (SOCKADDR*)&ipc[i], sizeof(ipc[i]));
  }

  //SENDS player start presets
  packet.playerData_[1].setPosition(kMax_width - packet.playerData_[1].getWidth(), 0);
  packet.ball = game_ball->getRect();
  for (int i = 0; i < players_number; ++i) {
    sendto(sock, (char*)&packet, sizeof(ServerPackage), 0, (SOCKADDR*)&ipc[i], sizeof(ipc[i]));
  }

  Vector2D ball_vector = { 0.5f, (rand() % 100 + 1) * 0.01f };
  //MAIN game loop
  bool client_close_request[2] = { false };
  bool* close_request = client_close_request;
  auto start = std::chrono::steady_clock::now();
  while (!close_request[0] && !close_request[1]) {
    switch (packet.game_state) {
    case kGameState_Countdown: {
      if (CountDown(sock, ipc, &packet, start))
        game_ball->reset();
      break;
    }
    case kGameState_Playing: {
      close_request = GameLoop(sock, ipc, &packet, game_ball);
      break;
    }
    default: {
      break;
    }
    }
  }

  bool p = close_request[1];
  //if conexion ended by player 1, p=0(p+1=1), else p=1(p+1=2)
  sprintf_s(packet.msg, "Conexion ended by player %d. Game finished\n", (int)(p + 1));
  packet.close_request = true;
  for (int i = 0; i < players_number; ++i) {
    sendto(sock, (char*)&packet, sizeof(ServerPackage), 0, (SOCKADDR*)&ipc[i], sizeof(ipc[i]));
  }

  int end_app = 0;
  delete(game_ball);
  recvfrom(sock, (char*)&end_app, sizeof(int), 0, NULL, NULL);
  recvfrom(sock, (char*)&end_app, sizeof(int), 0, NULL, NULL);
  shutdown(sock, SD_BOTH);
  closesocket(sock);
  WSACleanup();

  printf("Conexion finished");

  return 0;
}
