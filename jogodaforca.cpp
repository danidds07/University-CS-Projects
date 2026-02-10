#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_TENTATIVAS 6
#define MAX_PALAVRA 100

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void exibirPalavra(char palavra[], char letrasAdivinhadas[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (strchr(letrasAdivinhadas, palavra[i])) {
            printf("%c ", palavra[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

int main() {
    char palavras[3][MAX_PALAVRA] = {"PROGRAMACAO", "COMPUTADOR", "ALGORITMO"};
    char palavraSecreta[MAX_PALAVRA];
    char letrasAdivinhadas[MAX_PALAVRA] = "";
    int tentativas = 0;
    
    srand(time(NULL));
    strcpy(palavraSecreta, palavras[rand() % 3]);
    toLowerCase(palavraSecreta);
    int tamanho = strlen(palavraSecreta);

    printf("Jogo da Forca!\n");

    while (tentativas < MAX_TENTATIVAS) {
        printf("\nTentativas restantes: %d\n", MAX_TENTATIVAS - tentativas);
        exibirPalavra(palavraSecreta, letrasAdivinhadas, tamanho);

        printf("Digite uma letra: ");
        char letra;
        scanf(" %c", &letra);
        letra = tolower(letra);

        if (strchr(letrasAdivinhadas, letra)) {
            printf("Letra repetida. Tente outra.\n");
            continue;
        }

        letrasAdivinhadas[strlen(letrasAdivinhadas)] = letra;
        letrasAdivinhadas[strlen(letrasAdivinhadas) + 1] = '\0';

        if (strchr(palavraSecreta, letra) == NULL) {
            tentativas++;
            printf("Errou!\n");
        }

        int acertou = 1;
        for (int i = 0; i < tamanho; i++) {
            if (!strchr(letrasAdivinhadas, palavraSecreta[i])) {
                acertou = 0;
                break;
            }
        }

        if (acertou) {
            printf("Parabens! A palavra era: %s\n", palavraSecreta);
            break;
        }
    }

    if (tentativas == MAX_TENTATIVAS) {
        printf("Fim de jogo! A palavra era: %s\n", palavraSecreta);
    }

    return 0;
}
