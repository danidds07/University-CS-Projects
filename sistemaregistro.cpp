#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

typedef struct {
    int id;
    char nome[50];
    int idade;
    char curso[30];
    float nota;
    int ativo;
} Registro;

void clearScreen() { system("cls"); }
void waitKey() { printf("\nPressione qualquer tecla para voltar ao menu..."); _getch(); }

void trimNewline(char *s) {
    size_t n = strlen(s);
    if (n && (s[n-1] == '\n' || s[n-1] == '\r')) s[n-1] = '\0';
}

int isValidLetters(char *s) {
    int temLetra = 0;
    for (size_t i = 0; s[i]; i++) {
        unsigned char c = (unsigned char)s[i];
        if (isalpha(c)) temLetra = 1;
        if (!(isalpha(c) || isspace(c) || c=='\'' || c=='-' || c=='.')) return 0;
    }
    return temLetra;
}

int readLine(char *buf, size_t max) {
    if (!fgets(buf, (int)max, stdin)) return 0;
    trimNewline(buf);
    return 1;
}

int readInt(char *prompt) {
    char buf[128];
    char *endp;
    long v;
    for (;;) {
        printf("%s", prompt);
        if (!readLine(buf, sizeof buf)) continue;
        if (buf[0]=='\0') continue;
        v = strtol(buf, &endp, 10);
        if (*endp=='\0') return (int)v;
        printf("Entrada invalida. Digite um inteiro.\n");
    }
}

float readFloat(char *prompt) {
    char buf[128];
    char *endp;
    float v;
    for (;;) {
        printf("%s", prompt);
        if (!readLine(buf, sizeof buf)) continue;
        if (buf[0]=='\0') continue;
        v = strtof(buf, &endp);
        if (*endp=='\0') return v;
        printf("Entrada invalida. Digite um numero real.\n");
    }
}

void readLetters(char *prompt, char *out, size_t max) {
    for (;;) {
        printf("%s", prompt);
        if (!readLine(out, max)) continue;
        if (out[0]=='\0') { printf("Nao pode ficar vazio.\n"); continue; }
        if (isValidLetters(out)) return;
        printf("Entrada invalida. Use apenas letras, espacos, apostrofo, hifen e ponto.\n");
    }
}

long tamanho_arquivo(FILE *f) {
    long pos = ftell(f);
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fseek(f, pos, SEEK_SET);
    return tam;
}

void criptografarArquivo(char *entrada, char *saida) {
    FILE *fin = fopen(entrada, "rb");
    if (!fin) { remove(saida); return; }
    FILE *fout = fopen(saida, "wb");
    if (!fout) { fclose(fin); return; }
    int c;
    while ((c = fgetc(fin)) != EOF) fputc((unsigned char)c ^ 31, fout);
    fclose(fin);
    fclose(fout);
    fout = fopen(saida, "rb");
    if (!fout) return;
    long tam = tamanho_arquivo(fout);
    fclose(fout);
    if (tam <= 0) remove(saida);
}

void descriptografarArquivo(char *entrada) {
    FILE *fin = fopen(entrada, "rb");
    FILE *fout = fopen("ajuda_dec.txt", "wb");
    if(!fin || !fout) { if(fin) fclose(fin); if(fout) fclose(fout); return; }
    int c;
    while((c=fgetc(fin))!=EOF) fputc((unsigned char)c ^ 31, fout);
    fclose(fin);
    fclose(fout);
}

int arquivo_existe_e_tem_conteudo(char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    long tam = tamanho_arquivo(f);
    fclose(f);
    return tam > 0;
}

void ensureAjuda() {
    if (arquivo_existe_e_tem_conteudo("ajuda.txt")) return;
    char *padrao =
        "BEM-VINDO AO SISTEMA DE REGISTROS\n\n"
        "1 - Cadastrar: Adiciona um novo registro com ID, nome, idade, curso e nota.\n"
        "2 - Listar: Mostra todos os registros ativos.\n"
        "3 - Alterar: Edita um registro existente pelo ID.\n"
        "4 - Excluir logica: Marca registro como inativo, sem apagar do arquivo.\n"
        "5 - Excluir fisica: Remove definitivamente os inativos.\n"
        "6 - Consultar por ID: Procura registro pelo numero do ID.\n"
        "7 - Consultar por Nome: Procura pelo nome ou parte dele.\n"
        "8 - Atualizar ajuda: Regera o arquivo de ajuda criptografado.\n"
        "F1 - Ajuda: Exibe este manual.\n"
        "0 - Sair: Encerra o programa.\n\n"
        "OBS: Use numeros em campos numericos e apenas letras nos campos de texto.\n";
    FILE *orig = fopen("ajuda_original.txt", "wb");
    if (orig) { fwrite(padrao, 1, strlen(padrao), orig); fclose(orig); }
    criptografarArquivo("ajuda_original.txt", "ajuda.txt");
    if (!arquivo_existe_e_tem_conteudo("ajuda.txt")) {
        printf("\nFalha ao criar 'ajuda.txt'. Verifique permissoes e pasta de execucao.\n");
        waitKey();
    }
}

void mostrarAjuda() {
    clearScreen();
    ensureAjuda();
    descriptografarArquivo("ajuda.txt");
    printf("=== AJUDA ===\n\n");
    FILE *f = fopen("ajuda_dec.txt", "rb");
    if(f) {
        int c; while((c=fgetc(f))!=EOF) putchar(c);
        fclose(f);
        remove("ajuda_dec.txt");
    } else {
        printf("Arquivo de ajuda nao encontrado.\n");
    }
    waitKey();
}

void atualizarAjuda() {
    clearScreen();
    if (!arquivo_existe_e_tem_conteudo("ajuda_original.txt")) {
        ensureAjuda();
        if (arquivo_existe_e_tem_conteudo("ajuda.txt")) printf("Ajuda padrao criada e criptografada.\n");
        else printf("Falha ao criar ajuda padrao.\n");
        waitKey();
        return;
    }
    criptografarArquivo("ajuda_original.txt", "ajuda.txt");
    if (arquivo_existe_e_tem_conteudo("ajuda.txt")) printf("Ajuda criptografada gerada/atualizada com sucesso.\n");
    else printf("Falha ao gerar ajuda criptografada. Cheque 'ajuda_original.txt'.\n");
    waitKey();
}

void cadastrar() {
    clearScreen();
    FILE *f = fopen("registros.bin", "ab");
    if(!f) { printf("Erro ao abrir arquivo.\n"); waitKey(); return; }
    Registro r;
    r.id = readInt("ID: ");
    readLetters("Nome: ", r.nome, sizeof r.nome);
    r.idade = readInt("Idade: ");
    readLetters("Curso: ", r.curso, sizeof r.curso);
    r.nota = readFloat("Nota: ");
    r.ativo = 1;
    fwrite(&r, sizeof(Registro), 1, f);
    fclose(f);
    printf("Registro cadastrado.\n");
    waitKey();
}

void listar() {
    clearScreen();
    FILE *f = fopen("registros.bin", "rb");
    if(!f) { printf("Nenhum registro encontrado.\n"); waitKey(); return; }
    Registro r; int achou = 0;
    while(fread(&r, sizeof(Registro), 1, f)) {
        if(r.ativo) { printf("ID:%d | Nome:%s | Idade:%d | Curso:%s | Nota:%.2f\n", r.id, r.nome, r.idade, r.curso, r.nota); achou = 1; }
    }
    if (!achou) printf("Sem registros ativos.\n");
    fclose(f);
    waitKey();
}

void alterar() {
    clearScreen();
    FILE *f = fopen("registros.bin", "r+b");
    if(!f) { printf("Arquivo nao encontrado.\n"); waitKey(); return; }
    int id = readInt("Informe o ID para alterar: ");
    Registro r;
    while(fread(&r, sizeof(Registro), 1, f)) {
        if(r.id == id && r.ativo) {
            readLetters("Novo Nome: ", r.nome, sizeof r.nome);
            r.idade = readInt("Nova Idade: ");
            readLetters("Novo Curso: ", r.curso, sizeof r.curso);
            r.nota = readFloat("Nova Nota: ");
            fseek(f, - (long)sizeof(Registro), SEEK_CUR);
            fwrite(&r, sizeof(Registro), 1, f);
            printf("Registro alterado.\n");
            fclose(f);
            waitKey();
            return;
        }
    }
    printf("ID nao encontrado.\n");
    fclose(f);
    waitKey();
}

void excluirLogica() {
    clearScreen();
    FILE *f = fopen("registros.bin", "r+b");
    if(!f) { printf("Arquivo nao encontrado.\n"); waitKey(); return; }
    int id = readInt("Informe o ID para exclusao logica: ");
    Registro r;
    while(fread(&r, sizeof(Registro), 1, f)) {
        if(r.id == id && r.ativo) {
            r.ativo = 0;
            fseek(f, - (long)sizeof(Registro), SEEK_CUR);
            fwrite(&r, sizeof(Registro), 1, f);
            printf("Excluido logicamente.\n");
            fclose(f);
            waitKey();
            return;
        }
    }
    printf("ID nao encontrado.\n");
    fclose(f);
    waitKey();
}

void excluirFisica() {
    clearScreen();
    FILE *f = fopen("registros.bin", "rb");
    if(!f) { printf("Arquivo nao encontrado.\n"); waitKey(); return; }
    FILE *temp = fopen("temp.bin", "wb");
    if(!temp) { fclose(f); printf("Erro ao criar arquivo temporario.\n"); waitKey(); return; }
    Registro r; int copiados = 0;
    while(fread(&r, sizeof(Registro), 1, f)) {
        if(r.ativo) { fwrite(&r, sizeof(Registro), 1, temp); copiados++; }
    }
    fclose(f);
    fclose(temp);
    remove("registros.bin");
    rename("temp.bin", "registros.bin");
    printf("Exclusao fisica concluida. Registros ativos: %d\n", copiados);
    waitKey();
}

void consultarPorID() {
    clearScreen();
    FILE *f = fopen("registros.bin", "rb");
    if(!f) { printf("Arquivo nao encontrado.\n"); waitKey(); return; }
    int id = readInt("Informe o ID: ");
    Registro r;
    while(fread(&r, sizeof(Registro), 1, f)) {
        if(r.id == id && r.ativo) {
            printf("Encontrado: ID:%d | Nome:%s | Idade:%d | Curso:%s | Nota:%.2f\n", r.id, r.nome, r.idade, r.curso, r.nota);
            fclose(f);
            waitKey();
            return;
        }
    }
    printf("Registro nao encontrado.\n");
    fclose(f);
    waitKey();
}

void consultarPorNome() {
    clearScreen();
    FILE *f = fopen("registros.bin", "rb");
    if(!f) { printf("Arquivo nao encontrado.\n"); waitKey(); return; }
    char nome[50];
    readLetters("Informe o Nome: ", nome, sizeof nome);
    Registro r; int achou = 0;
    while(fread(&r, sizeof(Registro), 1, f)) {
        if(r.ativo && strstr(r.nome, nome)) {
            printf("ID:%d | Nome:%s | Idade:%d | Curso:%s | Nota:%.2f\n", r.id, r.nome, r.idade, r.curso, r.nota);
            achou = 1;
        }
    }
    if(!achou) printf("Nenhum registro correspondente.\n");
    fclose(f);
    waitKey();
}

int main() {
    int opcao, ext;
    ensureAjuda();
    do {
        clearScreen();
        printf("=== SISTEMA DE REGISTROS ===\n");
        printf("1 - Cadastrar\n");
        printf("2 - Listar\n");
        printf("3 - Alterar\n");
        printf("4 - Excluir (logica)\n");
        printf("5 - Excluir (fisica)\n");
        printf("6 - Consultar por ID\n");
        printf("7 - Consultar por Nome\n");
        printf("8 - Atualizar ajuda (criptografar)\n");
        printf("F1 - Ajuda\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        opcao = _getch();
        if (opcao == 0 || opcao == 224) {
            ext = _getch();
            if (ext == 59) { mostrarAjuda(); }
            else { continue; }
        } else {
            switch(opcao) {
                case '1': cadastrar(); break;
                case '2': listar(); break;
                case '3': alterar(); break;
                case '4': excluirLogica(); break;
                case '5': excluirFisica(); break;
                case '6': consultarPorID(); break;
                case '7': consultarPorNome(); break;
                case '8': atualizarAjuda(); break;
                case '0': break;
                default: printf("\nOpcao invalida.\n"); waitKey();
            }
        }
    } while(opcao != '0');
    clearScreen();
    printf("Saindo...\n");
    return 0;
}