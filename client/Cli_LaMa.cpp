// Adicionar no coompilador -lws2_32

#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>

#define MAX_MSG 1024
#define IP_SERVIDOR "127.0.0.1"
#define PORTA_SERVIDOR 2022

struct dados_s
{
  char buffer_arquivo[MAX_MSG];
  int qtd_bytes;
  int flag;
} typedef dados_t;

int main()
{
  FILE *arquivo;
  dados_t enviar;
  char caminho_arquivo[100];
  int bytes_lidos_arquivo, tam_endereco_servidor, bytes_recedibos;
  long tam_arquivo;

  char resposta[200];

  int socket_udp, bytes_enviados;
  struct sockaddr_in endereco_servidor;
  WSADATA infos_windows_sockets;
  LPHOSTENT infos_host_servidor;

  // Inicia a biblioteca de sockets do Windows
  WSAStartup(MAKEWORD(2, 1), &infos_windows_sockets);

  // obtendo o endereco do servidor
  infos_host_servidor = gethostbyname(IP_SERVIDOR);

  // criacao do socket
  socket_udp = socket(AF_INET, SOCK_DGRAM, 0);

  if (socket_udp < 0)
  {
    printf("\nNao foi possivel criar o socket com o servidor %s:\n", IP_SERVIDOR);
    exit(1);
    
  }

  // configuracao do endereco do servidor
  endereco_servidor.sin_family = AF_INET; // Define o tipo de endereco como IPv4
  endereco_servidor.sin_addr = *((LPIN_ADDR)*infos_host_servidor->h_addr_list); // Obtém o endereço IP do servidor
  endereco_servidor.sin_port = htons(PORTA_SERVIDOR); // Define a porta do servidor, convertendo de host para rede

  int i, fim = 10000000;
  for (i = 1; i < fim; i++)
  {
    //===========================
    // Leitura do arquivo
    //===========================

    printf("Digite o caminho do arquivo: ");
    scanf("%99[^\n]", caminho_arquivo); getchar(); // le o caminho do arquivo, limitando a 99 caracteres e removendo o '\n'

    // abre o arquivo
    arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL)
    {
      printf("ERRO: Nao foi possivel abrir o arquivo %s\n", caminho_arquivo);
      exit(1);
    }
    
    // le o arquivo
    bytes_lidos_arquivo = fread(enviar.buffer_arquivo, sizeof(char), sizeof(enviar.buffer_arquivo), arquivo);

    // fecha o arquivo
    fclose(arquivo);

    enviar.qtd_bytes = bytes_lidos_arquivo;

    //============================
    // Leitura da flag
    //============================
    printf("\nInforme\n1 - para soma\n2 - para media\n3 - para maior/menor");
    printf("\nDigite a flag: ");
    scanf("%d", &enviar.flag); getchar();                   

    //============================
    // Envio para o servidor
    //============================

    printf("\nEnviando arquivo %s e flag %d\n", caminho_arquivo, enviar.flag);

    // Envia os dados para o servidor
    bytes_enviados = sendto(socket_udp, (const char *)&enviar, sizeof(enviar), 0, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor));

    
    if (bytes_enviados <= 0)
    {
      printf("ERRO: Nao foi possivel enviar os dados %d\n", i - 1);
      exit(1);
    }

    printf("\nArquivo %s e flag %d enviados\n", caminho_arquivo, enviar.flag);

    //============================
    // Resposta do servidor
    //============================
    
    tam_endereco_servidor = sizeof(endereco_servidor);

    // Recebe a resposta do servidor
    printf("\nAguardando resposta do servidor...\n");
    bytes_recedibos = recvfrom(socket_udp, (char *)resposta, sizeof(resposta), 0, (struct sockaddr *)&endereco_servidor, &tam_endereco_servidor);

    if (bytes_recedibos < 0) 
    {
      printf("\nERRO: Nao foi possivel receber resposta do servidor.\n");
      exit(1);
    }

    printf("\nResposta do servidor: %s\n", resposta);
  }

  closesocket(socket_udp);
  WSACleanup();
  return 1;
}
