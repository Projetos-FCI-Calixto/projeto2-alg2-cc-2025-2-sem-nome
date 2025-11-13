#include <stdio.h>
#include <string.h>
#include <ctype.h>

// --- Constantes ---
#define MAX_ALTURA 768
#define MAX_LARGURA 1024
#define COR_BRANCO 0
#define COR_PRETO 1
#define COR_MISTA -1

// --- Protótipos ---
void mostrar_ajuda();
int ler_arquivo_pbm(const char* nome_arquivo, int imagem[MAX_ALTURA][MAX_LARGURA], int *ponteiro_largura, int *ponteiro_altura);
int verificar_se_e_homogenea(int imagem[MAX_ALTURA][MAX_LARGURA], int linha_inicio, int coluna_inicio, int altura_bloco, int largura_bloco);
void codificar_imagem(int imagem[MAX_ALTURA][MAX_LARGURA], int linha_inicio, int coluna_inicio, int altura_bloco, int largura_bloco);

// Variável estática para evitar estouro de pilha (Stack Overflow)
static int imagem[MAX_ALTURA][MAX_LARGURA];

int main(int num_argumentos, char *vetor_argumentos[]) {
    int largura = 0;
    int altura = 0;
    int sucesso_leitura = 0;

    if (num_argumentos == 1) {
        printf("Erro: Nenhum argumento fornecido.\n");
        mostrar_ajuda();
        return 1;
    }

    // Verifica flags
    if (strcmp(vetor_argumentos[1], "-?") == 0 || strcmp(vetor_argumentos[1], "--help") == 0) {
        mostrar_ajuda();
        return 0;
    } 
    else if (strcmp(vetor_argumentos[1], "-f") == 0 || strcmp(vetor_argumentos[1], "--file") == 0) {
        // --- SEU TRECHO: MODO ARQUIVO ---
        if (num_argumentos < 3) {
            printf("Erro: A opção -f requer um nome de arquivo.\n");
            return 1;
        }
        sucesso_leitura = ler_arquivo_pbm(vetor_argumentos[2], imagem, &largura, &altura);
    } 
    else if (strcmp(vetor_argumentos[1], "-m") == 0 || strcmp(vetor_argumentos[1], "--manual") == 0) {
        // --- TRECHO DA DUPLA (Placeholder) ---
        printf("Aviso: Modo manual será implementado pela dupla no próximo commit.\n");
        return 0; 
    } 
    else {
        printf("Erro: Opção desconhecida '%s'\n", vetor_argumentos[1]);
        return 1;
    }

    // Se a leitura funcionou, executa a codificação
    if (sucesso_leitura) {
        printf("Código gerado: ");
        codificar_imagem(imagem, 0, 0, altura, largura);
        printf("\n");
    } else {
        printf("Falha ao processar a imagem.\n");
        return 1;
    }

    return 0;
}

// --- SUAS FUNÇÕES (LEITURA PBM + LÓGICA PRINCIPAL) ---

void mostrar_ajuda() {
    printf("Uso: ImageEncoder [-? | -f ARQ | -m]\n");
    printf("  -f, --file: Ler arquivo PBM.\n");
    printf("  -m, --manual: Entrada manual (A fazer).\n");
}

int ler_arquivo_pbm(const char* nome_arquivo, int imagem[MAX_ALTURA][MAX_LARGURA], int *ponteiro_largura, int *ponteiro_altura) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível abrir o arquivo '%s'\n", nome_arquivo);
        return 0;
    }

    char buffer_leitura[100];
    fscanf(arquivo, "%s", buffer_leitura);
    if (strcmp(buffer_leitura, "P1") != 0) {
        printf("Erro: O arquivo não é um PBM (P1) válido.\n");
        fclose(arquivo);
        return 0;
    }

    // Ignorar comentários
    int caractere;
    while ((caractere = fgetc(arquivo)) != EOF) {
        if (isspace(caractere)) continue;
        if (caractere == '#') {
            while ((caractere = fgetc(arquivo)) != EOF && caractere != '\n' && caractere != '\r');
            continue;
        }
        ungetc(caractere, arquivo);
        break;
    }

    if (fscanf(arquivo, "%d %d", ponteiro_largura, ponteiro_altura) != 2) {
         printf("Erro: Falha ao ler dimensões.\n");
         fclose(arquivo);
         return 0;
    }

    // Validação básica
    if (*ponteiro_altura > MAX_ALTURA || *ponteiro_largura > MAX_LARGURA) {
        printf("Erro: Imagem muito grande.\n");
        fclose(arquivo);
        return 0;
    }

    for (int i = 0; i < *ponteiro_altura; i++) {
        for (int j = 0; j < *ponteiro_largura; j++) {
            if (fscanf(arquivo, "%d", &imagem[i][j]) != 1) {
                printf("Erro: Fim inesperado do arquivo.\n");
                fclose(arquivo);
                return 0;
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' lido com sucesso (%dx%d).\n", nome_arquivo, *ponteiro_largura, *ponteiro_altura);
    return 1;
}

int verificar_se_e_homogenea(int imagem[MAX_ALTURA][MAX_LARGURA], int linha_inicio, int coluna_inicio, int altura_bloco, int largura_bloco) {
    if (altura_bloco <= 0 || largura_bloco <= 0) return COR_MISTA;
    int cor_base = imagem[linha_inicio][coluna_inicio];
    
    for (int i = linha_inicio; i < linha_inicio + altura_bloco; i++) {
        for (int j = coluna_inicio; j < coluna_inicio + largura_bloco; j++) {
            if (imagem[i][j] != cor_base) return COR_MISTA;
        }
    }
    return cor_base;
}

void codificar_imagem(int imagem[MAX_ALTURA][MAX_LARGURA], int linha_inicio, int coluna_inicio, int altura_bloco, int largura_bloco) {
    int cor = verificar_se_e_homogenea(imagem, linha_inicio, coluna_inicio, altura_bloco, largura_bloco);

    if (cor == COR_BRANCO) { printf("B"); return; }
    if (cor == COR_PRETO) { printf("P"); return; }

    printf("X");

    int altura_sup = (altura_bloco + 1) / 2;
    int altura_inf = altura_bloco / 2;
    int largura_esq = (largura_bloco + 1) / 2;
    int largura_dir = largura_bloco / 2;

    // Q1, Q2, Q3, Q4
    if (altura_sup > 0 && largura_esq > 0) codificar_imagem(imagem, linha_inicio, coluna_inicio, altura_sup, largura_esq);
    if (altura_sup > 0 && largura_dir > 0) codificar_imagem(imagem, linha_inicio, coluna_inicio + largura_esq, altura_sup, largura_dir);
    if (altura_inf > 0 && largura_esq > 0) codificar_imagem(imagem, linha_inicio + altura_sup, coluna_inicio, altura_inf, largura_esq);
    if (altura_inf > 0 && largura_dir > 0) codificar_imagem(imagem, linha_inicio + altura_sup, coluna_inicio + largura_esq, altura_inf, largura_dir);
}