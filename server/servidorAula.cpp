//Adicionar no coompilador -lws2_32


#include <windows.h>
#include <winsock.h>

#include <stdio.h>
#include <string.h>

#define LOCAL_SERVER_PORT 12000
#define MAX_MSG 100

struct dados_s{
	char nome[40]; 
	char email[40]; 
	} typedef dados_t;

int main(int argc, char *argv[]) {

	dados_t recebido; 

  int pnt_sck, pnt_bind, num_bytes, tam_msg;
  
  struct sockaddr_in cliAddr, servAddr;
  
  char msg[MAX_MSG];

  WSADATA wsaData;
  WSAStartup(MAKEWORD(2,1),&wsaData);

  pnt_sck=socket(AF_INET, SOCK_DGRAM, 0);


  if(pnt_sck<0) {
    printf("\n %s: Nao foi possivel abrir o socket \n",argv[0]);
    exit(1);
  }

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(LOCAL_SERVER_PORT);
  
  pnt_bind = bind (pnt_sck, (struct sockaddr *) &servAddr,sizeof(servAddr));

  if(pnt_bind<0) {
    printf("\n Nao foi possivel associar a porta %d \n", LOCAL_SERVER_PORT);
    exit(1);
  }

  printf("\n Iniciando o Servidor: \n %s \n", argv[0]   );

  printf("\nAguardando dados na porta UDP: %u\n", LOCAL_SERVER_PORT);

  while(1) {
    //memset(msg,0x0,MAX_MSG);
    tam_msg = sizeof(cliAddr);
    
	num_bytes = recvfrom(pnt_sck, (char*)&recebido, sizeof(recebido), 0, (struct sockaddr *) &cliAddr, &tam_msg);
    
	if(num_bytes<0) {
      printf("\n Nao foi possivel receber dados \n");
      continue;
    }
    printf("\n Recebido de %s e %s: Porta %u IP: %s \n", recebido.nome, recebido.email, ntohs(cliAddr.sin_port), inet_ntoa(cliAddr.sin_addr) );


  //respondendo
  char resposta[] = "Oi - servidor.";
	
  pnt_bind = sendto(pnt_sck,(const char*)&resposta, sizeof(resposta), 0,(LPSOCKADDR) &cliAddr, sizeof(struct sockaddr));


 if(pnt_bind<=0) {
	  	printf("ERRO");
 } else{
	printf("Sucesso, o servidor enviou corretamente para o cliente a mesagem: %s\n", resposta);
}
//definindo a struct para receber o dado
typedef struct {
  int flag;
  char buffer[1024];
  unsigned int qtdBytes;
} manipulacaoDado;

//abrindo o arquivo
  FILE *arquivo;
  
  arquivo = fopen("arquivoCliente.txt", "r"); 

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
  } 
  
  while (fscanf(arquivo, sizeof(arquivo), arquivo) != NULL) {
    printf("%s", linha);
  }

  fclose(abrindoDados.arquivo);


// int valor;
//     while (fscanf(file_pointer, "%d", &valor) != EOF) {
//         printf("Valor lido: %d\n", valor);


}  	 
  
 closesocket(pnt_sck);
 WSACleanup();
return 0;
}

