#include <stdio.h>
#include <string.h>

int main()
{
    FILE *arquivo;
    char buffer[1024];
    int bytes_lidos;

    arquivo = fopen("x.txt", "r");
    if (arquivo == NULL)
    {
        printf("ERRO: Nao foi possivel abrir o arquivo x.txt\n");
        return 1;
    }

    bytes_lidos = fread(buffer, sizeof(char), sizeof(buffer) - 1, arquivo);
    if (bytes_lidos < 0)
    {
        printf("ERRO: Nao foi possivel ler o arquivo x.txt\n");
        fclose(arquivo);
        return 1;
    }

    
}