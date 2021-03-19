// PongServer.cpp

#include "SDL.h"
#include <stdio.h>
#include "ws2tcpip.h"

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

  printf("Server online, waiting for players\n");
  while (players_number < 2) {
    memset(new_client, 0, sizeof(new_client));
    recvfrom(sock, new_client, sizeof(new_client), 0,
      (SOCKADDR*)&ipc[players_number], &ip_size);

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

  return 0;
}
