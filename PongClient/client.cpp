// PongClient.cpp

#include "SDL.h"
#include <stdio.h>
#include "player.h"
#include "ws2tcpip.h"

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

int main(int argc, char** argv)
{
  SOCKET sock;
  struct sockaddr_in ip;
  Player player;
  int ip_size = sizeof(ip);
  player.init(EstablishConection(&sock, &ip));
  bool close_request = false;

  return 0;
}