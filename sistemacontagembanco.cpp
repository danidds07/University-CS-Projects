// Senha: 1203

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

char *unidades[] = {"", "um", "dois", "tres", "quatro", "cinco", "seis", "sete", "oito", "nove"};
char *dezenas10[] = {"dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezessete", "dezoito", "dezenove"};
char *dezenas[] = {"", "", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa"};
char *centenas[] = {"", "cento", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos"};

int ler_int_basico(const char *prompt, int min, int max) {
    char buf[128];
    int ok = 0, val = 0;
    while (!ok) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        buf[strcspn(buf, "\n")] = '\0';
        char *p = buf;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0') continue;
        char *q = p + strlen(p) - 1;
        while (q > p && isspace((unsigned char)*q)) q--;
        q[1] = '\0';
        int so_digitos = 1;
        for (char *r = p; *r; r++) {
            if (!isdigit((unsigned char)*r)) { so_digitos = 0; break; }
        }
        if (!so_digitos) continue;
        val = atoi(p);
        if (val < min || val > max) continue;
        ok = 1;
    }
    return val;
}

long long ler_centavos(const char *prompt, long long max) {
    char buf[128];
    int ok = 0;
    long long centavos = 0;
    while (!ok) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        buf[strcspn(buf, "\n")] = '\0';
        char *p = buf;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0') continue;
        char *q = p + strlen(p) - 1;
        while (q > p && isspace((unsigned char)*q)) q--;
        q[1] = '\0';

        int sep_count = 0, dec_count = 0, after_sep = 0, valido = 1, tem_digito = 0;
        for (char *r = p; *r; r++) {
            if (isdigit((unsigned char)*r)) {
                tem_digito = 1;
                if (after_sep) {
                    dec_count++;
                    if (dec_count > 2) { valido = 0; break; }
                }
            } else if (*r == '.' || *r == ',') {
                sep_count++;
                if (sep_count > 1) { valido = 0; break; }
                after_sep = 1;
            } else { valido = 0; break; }
        }
        if (!valido || !tem_digito) continue;

        for (char *r = p; *r; r++) if (*r == ',') *r = '.';
        double valor = atof(p);
        if (valor < 0.0 || valor > max) continue;
        centavos = (long long)(valor * 100 + 0.5);
        ok = 1;
    }
    return centavos;
}

void numeroPorExtenso(int n, char *saida) {
    if (n == 0) { strcpy(saida, "zero"); return; }
    if (n == 100) { strcpy(saida, "cem"); return; }
    char temp[500] = "";
    if (n / 100 > 0) {
        strcat(temp, centenas[n/100]);
        n %= 100;
        if (n > 0) strcat(temp, " e ");
    }
    if (n >= 20) {
        strcat(temp, dezenas[n/10]);
        n %= 10;
        if (n > 0) strcat(temp, " e ");
    }
    if (n >= 10 && n < 20) {
        strcat(temp, dezenas10[n-10]);
        n = 0;
    }
    if (n > 0) strcat(temp, unidades[n]);
    strcpy(saida, temp);
}

void numeroGrandePorExtenso(int n, char *saida) {
    char temp[1000] = "", parte[200];
    if (n >= 1000000) { strcpy(saida, "um milhao"); return; }

    int milhar = n / 1000;
    int resto = n % 1000;

    if (milhar > 0) {
        if (milhar == 1) {
            strcat(temp, "mil");
        } else {
            numeroPorExtenso(milhar, parte);
            strcat(temp, parte);
            strcat(temp, " mil");
        }
        if (resto > 0) {
            if (resto < 100) strcat(temp, " e ");
            else strcat(temp, " ");
        }
    }

    if (resto > 0) {
        numeroPorExtenso(resto, parte);
        strcat(temp, parte);
    }

    strcpy(saida, temp);
}

void opt1() {
    system("pause");
    system("cls");
    printf("--------------------------------- Banco Nacional das Ilhas Bitlandia! ---------------------------------\n\n");
    printf("\t1 - Saque em notas e moedas:\n");
    long long valor = ler_centavos("\nDigite o valor a sacar (0 a 1000000): ", 1000000);
    if (valor == 0) {
        printf("\nNenhuma nota ou moeda a dispensar.\n");
        return;
    }
    int notas[] = {10000, 5000, 2000, 1000, 500, 200};
    int moedas[] = {100, 50, 10, 1};
    int qtdNotas[6] = {0}, qtdMoedas[4] = {0};
    long long resto = valor;
    for (int i = 0; i < 6; i++) {
        qtdNotas[i] = resto / notas[i];
        resto %= notas[i];
    }
    for (int i = 0; i < 4; i++) {
        qtdMoedas[i] = resto / moedas[i];
        resto %= moedas[i];
    }
    printf("\nValor:\n");
    for (int i = 0; i < 6; i++) if (qtdNotas[i] > 0)
        printf("\t%2d nota(s) de B$%6.2f\n", qtdNotas[i], notas[i]/100.0);
    for (int i = 0; i < 4; i++) if (qtdMoedas[i] > 0)
        printf("\t%2d moeda(s) de B$%6.2f\n", qtdMoedas[i], moedas[i]/100.0);
}

void opt2() {
    system("pause");
    system("cls");
    printf("--------------------------------- Banco Nacional das Ilhas Bitlandia! ---------------------------------\n\n");
    printf("\t2 - Preenchimento de cheque:\n");
    long long centavos = ler_centavos("\nDigite o valor do cheque (0 a 1000000): ", 1000000);
    int reais = centavos / 100;
    int cent = centavos % 100;
    printf("\n\tCheque no valor de: B$ %d.%02d\n", reais, cent);
    char extenso[1000] = "";
    if (reais > 0) {
        numeroGrandePorExtenso(reais, extenso);
        printf("\tValor por extenso: %s Bit%s", extenso, (reais > 1 ? "s" : ""));
    } else {
        printf("\tValor por extenso: zero Bits");
    }
    if (cent > 0) {
        char extC[200];
        numeroPorExtenso(cent, extC);
        printf(" e %s CentBit%s", extC, (cent > 1 ? "s" : ""));
    }
    time_t agora = time(NULL);
    struct tm *hoje = localtime(&agora);
    printf("\n\tData: %02d/%02d/%04d\n", hoje->tm_mday, hoje->tm_mon + 1, hoje->tm_year + 1900);
}

int main() {
    int senha = ler_int_basico("Digite a senha de acesso: ", 0, 999999);
    if (senha != 1203) {
        printf("\n\tSenha incorreta!\n\nENCERRANDO SISTEMA . . .\n");
        return 0;
    }
    int repetir;
    do {
        int opt;
        do {
            system("cls");
            printf("--------------------------------- Bem-vindo ao Banco Nacional das Ilhas Bitlandia! ---------------------------------\n\n");
            printf("Digite o numero da opcao desejada:\n\n\t1 - Saque em notas e moedas\n\t2 - Preenchimento de cheque\n\n");
            opt = ler_int_basico("Opcao: ", 1, 2);
            if (opt < 1 || opt > 2) {
                printf("\nOpcao invalida! Digite novamente...\n");
                system("pause");
            }
        } while (opt < 1 || opt > 2);
        if (opt == 1) opt1(); else opt2();
        repetir = ler_int_basico("\nDeseja fazer outra acao (1-sim | 2-nao): ", 1, 2);
    } while (repetir == 1);
    printf("\n\nENCERRANDO SISTEMA . . .\n");
    return 0;
}