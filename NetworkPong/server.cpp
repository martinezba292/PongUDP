// PongServer.cpp

#include <stdio.h>
#include "common_def.h"

ClientPackage RecieveFromClient(const SOCKET& sock) {
  ClientPackage player_info;
  recvfrom(sock, (char*)&player_info, sizeof(ClientPackage), 0, NULL, NULL);
  return player_info;
}

void Update(ServerPackage* packet) {
  packet->playerData_[0].movePosition();
  packet->playerData_[1].movePosition();
}

int main(int argc, char** argv)
{
  WSADATA wsa;
  SOCKET sock;
  struct sockaddr_in ip, ipc[2];
  WSAStartup(MAKEWORD(2, 0), &wsa);
  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  ip.sin_family = AF_INET;
  InetPton(AF_INET, TEXT("0.0.0.0"), &ip.sin_addr.s_addr);
  ip.sin_port = htons(9999);
  bind(sock, (SOCKADDR*)&ip, sizeof(ip));
  char new_client[254];
  int ip_size = sizeof(ipc[0]);
  int players_number = 0;
  ServerPackage packet;

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
    
  printf("Starting game\n");
  memset(new_client, 0, sizeof(new_client));
  strcpy_s(new_client, "Begin!!\n");
  for (int i = 0; i < players_number; ++i) {
    sendto(sock, new_client, sizeof(new_client), 0, (SOCKADDR*)&ipc[i], sizeof(ipc[i]));
  }

  packet.playerData_[1].setPosition(kMax_width - packet.playerData_[1].getWidth(), 0);
  for (int i = 0; i < players_number; ++i) {
    sendto(sock, (char*)&packet, sizeof(ServerPackage), 0, (SOCKADDR*)&ipc[i], sizeof(ipc[i]));
  }

  while (1) {
    ClientPackage client1 = RecieveFromClient(sock);
    ClientPackage client2 = RecieveFromClient(sock);
    packet.playerData_[client1.playerData_.getTag()] = client1.playerData_;
    packet.playerData_[client2.playerData_.getTag()] = client2.playerData_;
    Update(&packet);

    for (int i = 0; i < players_number; ++i) {
      sendto(sock, (char*)&packet, sizeof(ServerPackage), 0, (SOCKADDR*)&ipc[i], sizeof(ipc[i]));
    }
  }

  return 0;
}
