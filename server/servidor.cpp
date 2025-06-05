// Adicionar no coompilador -lws2_32

#include <windows.h>
#include <winsock.h>

#include <stdio.h>

#define MAX_MSG (1024 - sizeof(int) * 2)
#define PORTA_SERVIDOR 2022

struct dados_s
{
  char buffer_arquivo[MAX_MSG];
  int qtd_bytes;
  int flag;
} typedef dados_t;

int main(int argc, char *argv[])
{

  dados_t recebido;

  int socket_udp, ligacao_socket_servidor, bytes_recebidos, bytes_enviados, tam_endereco_cliente;

  struct sockaddr_in endereco_cliente, endereco_servidor;

  char msg[MAX_MSG];

  WSADATA infos_windows_sockets;
  WSAStartup(MAKEWORD(2, 1), &infos_windows_sockets);

  socket_udp = socket(AF_INET, SOCK_DGRAM, 0);

  if (socket_udp < 0)
  {
    printf("\n %s: Nao foi possivel abrir o socket \n", argv[0]);
    exit(1);
  }

  endereco_servidor.sin_family = AF_INET;
  endereco_servidor.sin_addr.s_addr = htonl(INADDR_ANY);
  endereco_servidor.sin_port = htons(PORTA_SERVIDOR);

  ligacao_socket_servidor = bind(socket_udp, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor));

  if (ligacao_socket_servidor < 0)
  {
    printf("\n Nao foi possivel associar a porta %d \n", PORTA_SERVIDOR);
    exit(1);
  }

  printf("\n Iniciando o Servidor: \n %s \n", argv[0]);

  printf("\nAguardando dados na porta UDP: %u\n", PORTA_SERVIDOR);

  while (1)
  {
    tam_endereco_cliente = sizeof(endereco_cliente);

    bytes_recebidos = recvfrom(socket_udp, (char *)&recebido, sizeof(recebido), 0, (struct sockaddr *)&endereco_cliente, &tam_endereco_cliente);

    if (bytes_recebidos < 0)
    {
      printf("\nNao foi possivel receber dados %d\n");
      continue;
    }

    unsigned short porta_cliente = ntohs(endereco_cliente.sin_port);
    char *ip_cliente = inet_ntoa(endereco_cliente.sin_addr);

    printf("\n Recebido arquivo\nPorta: %u - IP: %s \n", porta_cliente, ip_cliente);

    bytes_enviados = sendto(socket_udp, (const char *)&"Dados recebidos", sizeof("Dados recebidos"), 0, (struct sockaddr *)&endereco_cliente, tam_endereco_cliente);

    if (bytes_enviados < 0)
    {
      printf("\nNao foi possivel enviar dados para o cliente %s: Porta %u \n", ip_cliente, porta_cliente);
      continue;
    }
  }

  closesocket(socket_udp);
  WSACleanup();
  return 0;
}
