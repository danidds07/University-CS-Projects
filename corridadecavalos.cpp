#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_SECONDS(n) Sleep(n * 1000)
#else
    #include <unistd.h>
    #define SLEEP_SECONDS(n) sleep(n)
#endif

#define NUM_CAVALOS 3
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void mostrarPista(int posicoes[], int vencedor) {
    for (int i = 0; i < NUM_CAVALOS; i++) {
        printf("Cavalo %d: ", i + 1);
        for (int j = 0; j < posicoes[i]; j++) printf("-");
        if (i == vencedor)
            printf(GREEN "X" RESET "\n");
        else
            printf(" C\n");
    }
}

int corrida(int distancia, int cavaloEscolhido) {
    int posicoes[NUM_CAVALOS] = {0};
    int vencedor = -1;
    cavaloEscolhido--; 

    while (1) {
        limparTela();
        printf("=== CORRIDA EM ANDAMENTO ===\n");
        
        for (int i = 0; i < NUM_CAVALOS; i++) {
            posicoes[i] += rand() % 3 + 1;
            if (posicoes[i] >= distancia && vencedor == -1)
                vencedor = i;
        }

        mostrarPista(posicoes, vencedor);
        SLEEP_SECONDS(1);

        if (vencedor != -1) {
            printf("\n");
            if (vencedor == cavaloEscolhido) {
                printf(GREEN "PARABENS! Cavalo %d venceu!\n" RESET, vencedor + 1);
            } else {
                printf(RED "Que pena! Cavalo %d venceu.\n" RESET, vencedor + 1);
                printf(YELLOW "Voce havia escolhido o cavalo %d\n" RESET, cavaloEscolhido + 1);
            }
            return (vencedor == cavaloEscolhido);
        }
    }
}

int main() {
    srand(time(NULL));
    int nivel, distancia, cavalo;
    char jogarNovamente;
    int vitorias = 0, total_jogos = 0;

    do {
        limparTela();
        printf("=== CORRIDA DE CAVALOS ===\n");
        printf("Vitorias: %d/%d\n\n", vitorias, total_jogos);
        
        do {
            printf("Escolha a distancia da corrida:\n");
            printf("1. Curta (20)\n2. Media (35)\n3. Longa (50)\n");
            printf("Opcao: ");
            scanf("%d", &nivel);
        } while (nivel < 1 || nivel > 3);

        do {
            printf("\nEscolha seu cavalo (1 a 3): ");
            scanf("%d", &cavalo);
        } while (cavalo < 1 || cavalo > 3);

        distancia = (nivel == 1) ? 20 : (nivel == 2) ? 35 : 50;
        
        total_jogos++;
        if (corrida(distancia, cavalo)) {
            vitorias++;
        }

        printf("\nDeseja jogar novamente? (s/n): ");
        scanf(" %c", &jogarNovamente);
    } while (jogarNovamente == 's' || jogarNovamente == 'S');

    limparTela();
    printf("=== RESULTADO FINAL ===\n");
    printf("Voce acertou %d de %d corridas!\n", vitorias, total_jogos);
    printf("Obrigado por jogar!\n");

    return 0;
}
