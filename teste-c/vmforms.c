#include <stdio.h>

int main() {
    char nome[50];
    char email[50];
    char mensagem[300];

    printf("Digite seu nome: ");
    fgets(nome, sizeof(nome), stdin);

    printf("Digite seu email: ");
    fgets(email, sizeof(email), stdin);

    printf("Digite sua mensagem: ");
    fgets(mensagem, sizeof(mensagem), stdin);

    printf("Enviado com sucesso!\n");
    return 0;
}