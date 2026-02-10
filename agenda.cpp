#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct comp {
    int dia;
    int mes;
    int ano;
    int hora;
    int min;
    char desc[81];
    struct comp *prox;
} Comp;

void ler_linha(char *s, int tam) {
    if (fgets(s, tam, stdin)) {
        int n = strlen(s);
        if (n > 0 && s[n - 1] == '\n') {
            s[n - 1] = '\0';
        }
    }
}

int ler_int(char *msg) {
    char s[100];
    int x;
    char extra;
    for (;;) {
        printf("%s", msg);
        if (!fgets(s, sizeof(s), stdin)) {
            continue;
        }
        if (sscanf(s, "%d %c", &x, &extra) == 1) {
            return x;
        }
        printf("Entrada invalida. Digite apenas numeros.\n");
    }
}

void limpa_tela() {
    system("cls");
}

void pausa() {
    char s[4];
    printf("\nPressione ENTER para continuar...");
    fgets(s, sizeof(s), stdin);
}

int antes(Comp *a, Comp *b) {
    if (a->ano != b->ano) return a->ano < b->ano;
    if (a->mes != b->mes) return a->mes < b->mes;
    if (a->dia != b->dia) return a->dia < b->dia;
    if (a->hora != b->hora) return a->hora < b->hora;
    if (a->min != b->min) return a->min < b->min;
    return 0;
}

Comp *cria_comp(int dia, int mes, int ano, int hora, int min, char *desc) {
    Comp *n = (Comp *)malloc(sizeof(Comp));
    if (!n) return NULL;
    n->dia = dia;
    n->mes = mes;
    n->ano = ano;
    n->hora = hora;
    n->min = min;
    strncpy(n->desc, desc, 80);
    n->desc[80] = '\0';
    n->prox = NULL;
    return n;
}

void insere_ordenado(Comp **lista, Comp *n) {
    if (*lista == NULL || antes(n, *lista)) {
        n->prox = *lista;
        *lista = n;
    } else {
        Comp *p = *lista;
        while (p->prox != NULL && !antes(n, p->prox)) {
            p = p->prox;
        }
        n->prox = p->prox;
        p->prox = n;
    }
}

void inserir(Comp **lista) {
    int dia = ler_int("Dia: ");
    int mes = ler_int("Mes: ");
    int ano = ler_int("Ano: ");
    int hora = ler_int("Hora: ");
    int min = ler_int("Minuto: ");
    char desc[81];
    printf("Descricao: ");
    ler_linha(desc, 81);
    Comp *n = cria_comp(dia, mes, ano, hora, min, desc);
    if (n) {
        insere_ordenado(lista, n);
        printf("Compromisso inserido.\n");
    } else {
        printf("Erro de memoria.\n");
    }
}

void remover_data(Comp **lista) {
    int dia = ler_int("Dia: ");
    int mes = ler_int("Mes: ");
    int ano = ler_int("Ano: ");
    Comp *p = *lista;
    Comp *ant = NULL;
    int achou = 0;
    while (p != NULL) {
        if (p->dia == dia && p->mes == mes && p->ano == ano) {
            Comp *rem = p;
            if (ant == NULL) *lista = p->prox;
            else ant->prox = p->prox;
            p = p->prox;
            free(rem);
            achou = 1;
        } else {
            ant = p;
            p = p->prox;
        }
    }
    if (achou) printf("Compromissos removidos.\n");
    else printf("Nenhum compromisso nesta data.\n");
}

void remover_palavra(Comp **lista) {
    char palavra[81];
    int cont = 0;
    printf("Palavra: ");
    ler_linha(palavra, 81);
    Comp *p = *lista;
    while (p != NULL) {
        if (strstr(p->desc, palavra)) {
            cont++;
            printf("%d) %02d/%02d/%04d %02d:%02d - %s\n",
                   cont, p->dia, p->mes, p->ano, p->hora, p->min, p->desc);
        }
        p = p->prox;
    }
    if (cont == 0) {
        printf("Nenhum encontrado.\n");
        return;
    }
    int esc = ler_int("Escolha: ");
    if (esc <= 0 || esc > cont) {
        printf("Cancelado.\n");
        return;
    }
    p = *lista;
    Comp *ant = NULL;
    cont = 0;
    while (p != NULL) {
        if (strstr(p->desc, palavra)) {
            cont++;
            if (cont == esc) {
                if (ant == NULL) *lista = p->prox;
                else ant->prox = p->prox;
                free(p);
                printf("Removido.\n");
                return;
            }
        }
        ant = p;
        p = p->prox;
    }
}

void consultar_data(Comp *lista) {
    int dia = ler_int("Dia: ");
    int mes = ler_int("Mes: ");
    int ano = ler_int("Ano: ");
    int achou = 0;
    Comp *p = lista;
    while (p != NULL) {
        if (p->dia == dia && p->mes == mes && p->ano == ano) {
            printf("%02d:%02d - %s\n", p->hora, p->min, p->desc);
            achou = 1;
        }
        p = p->prox;
    }
    if (!achou) printf("Nenhum compromisso.\n");
}

void procurar_palavra(Comp *lista) {
    char palavra[81];
    int achou = 0;
    printf("Palavra: ");
    ler_linha(palavra, 81);
    Comp *p = lista;
    while (p != NULL) {
        if (strstr(p->desc, palavra)) {
            printf("%02d/%02d/%04d %02d:%02d - %s\n",
                   p->dia, p->mes, p->ano, p->hora, p->min, p->desc);
            achou = 1;
        }
        p = p->prox;
    }
    if (!achou) printf("Nenhum encontrado.\n");
}

void alterar_por_palavra(Comp **lista) {
    char palavra[81];
    int cont = 0;
    printf("Palavra: ");
    ler_linha(palavra, 81);
    Comp *p = *lista;
    while (p != NULL) {
        if (strstr(p->desc, palavra)) {
            cont++;
            printf("%d) %02d/%02d/%04d %02d:%02d - %s\n",
                   cont, p->dia, p->mes, p->ano, p->hora, p->min, p->desc);
        }
        p = p->prox;
    }
    if (cont == 0) {
        printf("Nenhum encontrado.\n");
        return;
    }
    int esc = ler_int("Escolha: ");
    if (esc <= 0 || esc > cont) {
        printf("Cancelado.\n");
        return;
    }
    p = *lista;
    Comp *ant = NULL;
    cont = 0;
    while (p != NULL) {
        if (strstr(p->desc, palavra)) {
            cont++;
            if (cont == esc) {
                if (ant == NULL) *lista = p->prox;
                else ant->prox = p->prox;
                int dia = ler_int("Dia: ");
                int mes = ler_int("Mes: ");
                int ano = ler_int("Ano: ");
                int hora = ler_int("Hora: ");
                int min = ler_int("Minuto: ");
                p->dia = dia;
                p->mes = mes;
                p->ano = ano;
                p->hora = hora;
                p->min = min;
                insere_ordenado(lista, p);
                printf("Alterado.\n");
                return;
            }
        }
        ant = p;
        p = p->prox;
    }
}

void alterar_data_hora(Comp *lista) {
    int dia = ler_int("Dia: ");
    int mes = ler_int("Mes: ");
    int ano = ler_int("Ano: ");
    int hora = ler_int("Hora: ");
    int min = ler_int("Minuto: ");
    int cont = 0;
    Comp *p = lista;
    while (p != NULL) {
        if (p->dia == dia && p->mes == mes && p->ano == ano &&
            p->hora == hora && p->min == min) {
            cont++;
            printf("%d) %02d/%02d/%04d %02d:%02d - %s\n",
                   cont, p->dia, p->mes, p->ano, p->hora, p->min, p->desc);
        }
        p = p->prox;
    }
    if (cont == 0) {
        printf("Nenhum encontrado.\n");
        return;
    }
    int esc = ler_int("Escolha: ");
    if (esc <= 0 || esc > cont) {
        printf("Cancelado.\n");
        return;
    }
    char nova[81];
    p = lista;
    cont = 0;
    while (p != NULL) {
        if (p->dia == dia && p->mes == mes && p->ano == ano &&
            p->hora == hora && p->min == min) {
            cont++;
            if (cont == esc) {
                printf("Nova descricao: ");
                ler_linha(nova, 81);
                strncpy(p->desc, nova, 80);
                p->desc[80] = '\0';
                printf("Alterado.\n");
                return;
            }
        }
        p = p->prox;
    }
}

void liberar_lista(Comp **lista) {
    Comp *p = *lista;
    while (p != NULL) {
        Comp *r = p;
        p = p->prox;
        free(r);
    }
    *lista = NULL;
}

void salvar(Comp *lista) {
    FILE *f = fopen("agenda.txt", "w");
    if (!f) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }
    Comp *p = lista;
    while (p != NULL) {
        fprintf(f, "%d %d %d %d %d|%s\n",
                p->dia, p->mes, p->ano, p->hora, p->min, p->desc);
        p = p->prox;
    }
    fclose(f);
    printf("Agenda salva.\n");
}

void carregar(Comp **lista) {
    FILE *f = fopen("agenda.txt", "r");
    if (!f) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }
    char linha[200];
    liberar_lista(lista);
    while (fgets(linha, 200, f)) {
        int dia, mes, ano, hora, min;
        char texto[81];
        char *sep = strchr(linha, '|');
        if (!sep) continue;
        *sep = '\0';
        if (sscanf(linha, "%d %d %d %d %d", &dia, &mes, &ano, &hora, &min) != 5)
            continue;
        strncpy(texto, sep + 1, 80);
        texto[80] = '\0';
        int n = strlen(texto);
        if (n > 0 && texto[n - 1] == '\n') texto[n - 1] = '\0';
        Comp *p = cria_comp(dia, mes, ano, hora, min, texto);
        if (p) insere_ordenado(lista, p);
    }
    fclose(f);
    printf("Agenda carregada.\n");
}

void tela_inserir(Comp **lista) {
    limpa_tela();
    printf("INSERIR COMPROMISSO\n\n");
    inserir(lista);
    pausa();
}

void tela_remover_data(Comp **lista) {
    limpa_tela();
    printf("REMOVER POR DATA\n\n");
    remover_data(lista);
    pausa();
}

void tela_remover_palavra(Comp **lista) {
    limpa_tela();
    printf("REMOVER POR PALAVRA\n\n");
    remover_palavra(lista);
    pausa();
}

void tela_consultar_data(Comp *lista) {
    limpa_tela();
    printf("CONSULTAR POR DATA\n\n");
    consultar_data(lista);
    pausa();
}

void tela_procurar_palavra(Comp *lista) {
    limpa_tela();
    printf("PROCURAR POR PALAVRA\n\n");
    procurar_palavra(lista);
    pausa();
}

void tela_alterar_por_palavra(Comp **lista) {
    limpa_tela();
    printf("ALTERAR DATA POR PALAVRA\n\n");
    alterar_por_palavra(lista);
    pausa();
}

void tela_alterar_data_hora(Comp *lista) {
    limpa_tela();
    printf("ALTERAR TEXTO POR DATA E HORA\n\n");
    alterar_data_hora(lista);
    pausa();
}

void tela_salvar(Comp *lista) {
    limpa_tela();
    printf("SALVAR AGENDA\n\n");
    salvar(lista);
    pausa();
}

void tela_carregar(Comp **lista) {
    limpa_tela();
    printf("CARREGAR AGENDA\n\n");
    carregar(lista);
    pausa();
}

int main() {
    Comp *lista = NULL;
    int op;
    do {
        limpa_tela();
        printf("--- AGENDA ---\n");
        printf("1 - Inserir\n");
        printf("2 - Remover por data\n");
        printf("3 - Remover por palavra\n");
        printf("4 - Consultar por data\n");
        printf("5 - Procurar por palavra\n");
        printf("6 - Alterar data por palavra\n");
        printf("7 - Alterar texto por data e hora\n");
        printf("8 - Salvar agenda\n");
        printf("9 - Carregar agenda\n");
        printf("10 - Sair\n");
        op = ler_int("Opcao: ");
        switch (op) {
            case 1: tela_inserir(&lista); break;
            case 2: tela_remover_data(&lista); break;
            case 3: tela_remover_palavra(&lista); break;
            case 4: tela_consultar_data(lista); break;
            case 5: tela_procurar_palavra(lista); break;
            case 6: tela_alterar_por_palavra(&lista); break;
            case 7: tela_alterar_data_hora(lista); break;
            case 8: tela_salvar(lista); break;
            case 9: tela_carregar(&lista); break;
        }
    } while (op != 10);
    liberar_lista(&lista);
}