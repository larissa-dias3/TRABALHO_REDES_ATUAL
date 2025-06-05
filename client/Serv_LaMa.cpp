// Adicionar no coompilador -lws2_32

#include <windows.h>
#include <winsock.h>

#include <stdio.h>
#include <limits.h>

#define MAX_MSG 1024
#define PORTA_SERVIDOR 2022

struct dados_s
{
  char buffer_arquivo[MAX_MSG];
  int qtd_bytes;
  int flag;
} typedef dados_t;

void processa_buffer(char *buffer, int tamanho, int flag, char *resultado)
{
  int total_numeros = 0, soma = 0, maior = INT_MIN, menor = INT_MAX;
  char buffer_temp[13];
  int index_temp = 0;
  float media = 0.0;

  for (int i = 0; i < tamanho; i++)
  {
    if (buffer[i] == '\n' || i == (tamanho - 1))
    {
      if (buffer[i] != '\n')
      {
        buffer_temp[index_temp] = buffer[i];
        index_temp++;
      }

      buffer_temp[index_temp] = '\0';
      index_temp = 0;
    }
    else
    {
      buffer_temp[index_temp] = buffer[i];
      index_temp++;
      continue;
    }

    int num = atoi(buffer_temp);
    total_numeros++;

    soma += num;

    if (num > maior)
      maior = num;
    if (num < menor)
      menor = num;
  }

  switch (flag)
  {
  case 1:
    sprintf(resultado, "Soma dos numeros: %d", soma); 
    break;
  case 2:
    sprintf(resultado, "Media dos numeros: %.2f", (float)soma / total_numeros);
    break;
  default:
    sprintf(resultado, "Maior numero: %d | Menor numero: %d", maior, menor);
    break;
  }
}

int main(int argc, char *argv[])
{
  dados_t recebido;

  int socket_udp, ligacao_socket_servidor, bytes_recebidos, bytes_enviados, tam_endereco_cliente;

  struct sockaddr_in endereco_cliente, endereco_servidor;

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

  while(1)
  {
    int aprovado = 1;
    char resposta[200];
    tam_endereco_cliente = sizeof(endereco_cliente);

    //===========================
    // Requisicao cliente
    //===========================

    bytes_recebidos = recvfrom(socket_udp, (char *)&recebido, sizeof(recebido), 0, (struct sockaddr *)&endereco_cliente, &tam_endereco_cliente);

    if (bytes_recebidos < 0)
    {
      printf("\nNao foi possivel receber dados\n");
      continue;
    }

    unsigned short porta_cliente = ntohs(endereco_cliente.sin_port);
    char *ip_cliente = inet_ntoa(endereco_cliente.sin_addr);

    printf("\n Recebido arquivo\nPorta: %u - IP: %s \n", porta_cliente, ip_cliente);

    //===========================
    // Processamento dados
    //===========================

    printf("\nProcessando buffer recebido: %d bytes, Flag: %d\n", recebido.qtd_bytes, recebido.flag);

    if (recebido.qtd_bytes > MAX_MSG)
    {
      aprovado = 0;
      strcpy(resposta, "Erro: Tamanho do buffer excede o limite permitido.");
      printf("\nErro: Tamanho do buffer excede o limite de %d bytes\n", MAX_MSG);
    }

    if (recebido.qtd_bytes <= 0)
    {
      aprovado = 0;
      strcpy(resposta, "Erro: Tamanho do buffer invalido.");
      printf("\nTamanho do buffer invalido: %d bytes\n", recebido.qtd_bytes);
    }

    if (recebido.flag < 1 || recebido.flag > 3)
    {
      aprovado = 0;
      strcpy(resposta, "Erro: Flag invalida. Deve ser 1, 2 ou 3.");
      printf("\nFlag invalida: %d. Deve ser 1, 2 ou 3.\n", recebido.flag);
    }

    if (aprovado)
      processa_buffer(recebido.buffer_arquivo, recebido.qtd_bytes, recebido.flag, resposta);

    //===========================
    // Resposta ao cliente
    //===========================

    printf("\nEnviando resposta para o cliente %s: Porta %u \n", ip_cliente, porta_cliente);

    bytes_enviados = sendto(socket_udp, (const char *)&resposta, sizeof(resposta), 0, (struct sockaddr *)&endereco_cliente, tam_endereco_cliente);

    if (bytes_enviados < 0)
    {
      printf("\nNao foi possivel enviar dados para o cliente %s: Porta %u \n", ip_cliente, porta_cliente);
      continue;
    }

    printf("\nResposta enviada: %s\n", resposta);
    printf("\n---------------------------------\n");
  }

  closesocket(socket_udp);
  WSACleanup();
  return 0;
}
