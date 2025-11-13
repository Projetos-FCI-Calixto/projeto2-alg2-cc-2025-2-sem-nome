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
/**
 * Projeto 2 - Codificador de Imagens Binárias
 * Disciplina: Algoritmos e Programação II
 * * Este programa implementa um codificador de imagens binárias (preto e branco)
 * usando um algoritmo recursivo de divisão por quadrantes.
 * * Aluno: [Seu Nome Aqui]
 */

#include <stdio.h>  // Para printf, scanf, FILE, fopen, fclose, etc.
#include <stdlib.h> // Para malloc, free, exit
#include <string.h> // Para strcmp (comparar strings)
#include <ctype.h>  // Para isspace (verificar espaço em branco)

// --- Constantes ---
#define COR_BRANCO 0
#define COR_PRETO 1
#define COR_MISTA -1 // Renomeado de COR_MISTURADA

// --- Protótipos das Funções (Declarações) ---

void mostrar_ajuda();
int** alocar_imagem(int altura, int largura);
void liberar_imagem(int** imagem, int altura);
int** ler_entrada_manual(int* ponteiro_largura, int* ponteiro_altura);
int** ler_arquivo_pbm(const char* nome_arquivo, int* ponteiro_largura, int* ponteiro_altura);
int verificar_se_e_homogenea(int** imagem, int linha_inicio, int coluna_inicio, int altura_bloco, int largura_bloco);
void codificar_imagem(int** imagem, int linha_inicio, int coluna_inicio, int altura_bloco, int largura_bloco);

// --- Função Principal (main) ---

/**
 * Ponto de entrada do programa.
 * Processa os argumentos da linha de comando e chama o modo apropriado.
 */
int main(int num_argumentos, char *vetor_argumentos[]) {
    int** imagem = NULL;
    int largura = 0;
    int altura = 0;

    // 1. Processar argumentos da linha de comando
    // Se num_argumentos é 1, significa que só o nome do programa foi digitado
    if (num_argumentos == 1) {
        printf("Erro: Nenhum argumento fornecido.\n\n");
        mostrar_ajuda();
        return 1; // Retorna com código de erro
    }

    // Compara o primeiro argumento (vetor_argumentos[1]) com as strings de ajuda
    if (strcmp(vetor_argumentos[1], "-?") == 0 || strcmp(vetor_argumentos[1], "--help") == 0) {
        mostrar_ajuda();
        return 0; // Sucesso
    }
    
    // Compara o primeiro argumento com as strings do modo manual
    if (strcmp(vetor_argumentos[1], "-m") == 0 || strcmp(vetor_argumentos[1], "--manual") == 0) {
        // Modo Manual
        imagem = ler_entrada_manual(&largura, &altura);
    }
    // Compara o primeiro argumento com as strings do modo arquivo
    else if (strcmp(vetor_argumentos[1], "-f") == 0 || strcmp(vetor_argumentos[1], "--file") == 0) {
        // Modo Arquivo
        // Precisa de 3 argumentos: ./programa -f nome_arquivo.pbm
        if (num_argumentos < 3) {
            printf("Erro: A opção -f requer um nome de arquivo.\n\n");
            mostrar_ajuda();
            return 1;
        }
        // O nome do arquivo é o segundo argumento (vetor_argumentos[2])
        imagem = ler_arquivo_pbm(vetor_argumentos[2], &largura, &altura);
    }
    else {
        printf("Erro: Opção desconhecida '%s'\n\n", vetor_argumentos[1]);
        mostrar_ajuda();
        return 1;
    }

    // 2. Verificar se a imagem foi carregada com sucesso
    if (imagem == NULL) {
        printf("Erro fatal: Não foi possível carregar ou alocar a imagem.\n");
        return 1;
    }
    
    // 3. Iniciar o processo de codificação
    printf("Código gerado: ");
    // A primeira chamada processa a imagem inteira (começa em 0,0)
    codificar_imagem(imagem, 0, 0, altura, largura);
    printf("\n"); // Imprime uma nova linha no final do código

    // 4. Limpar a memória que foi alocada
    liberar_imagem(imagem, altura);
    return 0; // Termina o programa com sucesso
}

// --- Implementação das Funções ---

/**
 * Exibe as instruções de uso do programa (CLI).
 */
void mostrar_ajuda() {
    printf("Uso: ImageEncoder [-? | -m | -f ARQ]\n");
    printf("Codifica imagens binárias dadas em arquivos PBM ou por dados informados\n");
    printf("manualmente.\n\n");
    printf("Argumentos:\n");
    printf("  -?, --help : apresenta essa orientação na tela.\n");
    printf("  -m, --manual: ativa o modo de entrada manual, em que o usuário fornece\n");
    printf("                todos os dados da imagem informando-os através do teclado.\n");
    printf("  -f, --file: considera a imagem representada no arquivo PBM (Portable\n");
    printf("              bitmap).\n");
}

/**
 * Aloca dinamicamente uma matriz 2D para a imagem.
 * Retorna um ponteiro para a matriz alocada (ponteiro para ponteiro).
 */
int** alocar_imagem(int altura, int largura) {
    if (altura <= 0 || largura <= 0) return NULL;

    // Aloca um array de ponteiros (um para cada linha)
    int** imagem = (int**)malloc(altura * sizeof(int*));
    if (imagem == NULL) return NULL; // Falha ao alocar memória

    // Aloca cada linha (um array de inteiros para as colunas)
    for (int i = 0; i < altura; i++) {
        imagem[i] = (int*)malloc(largura * sizeof(int));
        if (imagem[i] == NULL) {
            // Se falhar no meio, libera tudo que já foi alocado
            for (int j = 0; j < i; j++) {
                free(imagem[j]);
            }
            free(imagem);
            return NULL;
        }
    }
    return imagem; // Retorna o ponteiro para a matriz alocada
}

/**
 * Libera a memória alocada dinamicamente para a imagem.
 */
void liberar_imagem(int** imagem, int altura) {
    if (imagem == NULL) return; // Se for nulo, não faz nada
    
    // Libera cada linha primeiro
    for (int i = 0; i < altura; i++) {
        free(imagem[i]);
    }
    // Depois libera o array de ponteiros
    free(imagem);
}

/**
 * Lê os dados da imagem (dimensões e pixels) manualmente via teclado.
 * Retorna a imagem alocada e preenchida.
 */
int** ler_entrada_manual(int* ponteiro_largura, int* ponteiro_altura) {
    printf("--- Modo de Entrada Manual ---\n");
    printf("Digite a largura da imagem: ");
    scanf("%d", ponteiro_largura); // Armazena o valor no endereço apontado
    printf("Digite a altura da imagem: ");
    scanf("%d", ponteiro_altura); // Armazena o valor no endereço apontado

    // Validação de tamanho
    if (*ponteiro_altura <= 0 || *ponteiro_largura <= 0 || *ponteiro_altura > 768 || *ponteiro_largura > 1024) {
        printf("Erro: Dimensões inválidas (limite 1024x768).\n");
        return NULL;
    }

    // Aloca a imagem com o tamanho informado
    int** imagem = alocar_imagem(*ponteiro_altura, *ponteiro_largura);
    if (imagem == NULL) return NULL;

    printf("Digite os pixels (0 para BRANCO, 1 para PRETO), linha por linha:\n");
    for (int i = 0; i < *ponteiro_altura; i++) {
        printf("Linha %d: ", i + 1);
        for (int j = 0; j < *ponteiro_largura; j++) {
            scanf("%d", &imagem[i][j]); // Lê o pixel para a posição [i][j]
        }
    }
    
    printf("Leitura manual concluída.\n");
    return imagem;
}

/**
 * Lê uma imagem no formato PBM (P1).
 * Retorna a imagem alocada e preenchida.
 */
int** ler_arquivo_pbm(const char* nome_arquivo, int* ponteiro_largura, int* ponteiro_altura) {
    // Tenta abrir o arquivo em modo "r" (read / leitura)
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível abrir o arquivo '%s'\n", nome_arquivo);
        return NULL;
    }

    char buffer_leitura[100]; // Para ler o "magic number" (P1)
    
    // 1. Ler o "Magic Number" (P1)
    fscanf(arquivo, "%s", buffer_leitura);
    if (strcmp(buffer_leitura, "P1") != 0) {
        printf("Erro: O arquivo não é um PBM (P1) válido.\n");
        fclose(arquivo);
        return NULL;
    }

    // 2. Ignorar comentários e pular espaços em branco
    int caractere;
    while ((caractere = fgetc(arquivo)) != EOF) { // Lê um caractere
        if (isspace(caractere)) {
            continue; // Pula espaço em branco, tab, nova linha
        }
        if (caractere == '#') {
            // É um comentário, ignora até o fim da linha
            while ((caractere = fgetc(arquivo)) != EOF && caractere != '\n' && caractere != '\r');
            continue;
        }
        
        // Se não é espaço nem comentário, deve ser o número da largura
        ungetc(caractere, arquivo); // Devolve o caractere para o stream
        break; // Sai do loop para ler as dimensões
    }

    // 3. Ler largura e altura
    if (fscanf(arquivo, "%d %d", ponteiro_largura, ponteiro_altura) != 2) {
         printf("Erro: Formato PBM inválido (não foi possível ler dimensões).\n");
         fclose(arquivo);
         return NULL;
    }
    
    // Validação de tamanho
    if (*ponteiro_altura <= 0 || *ponteiro_largura <= 0 || *ponteiro_altura > 768 || *ponteiro_largura > 1024) {
        printf("Erro: Dimensões inválidas no arquivo (limite 1024x768).\n");
        fclose(arquivo);
        return NULL;
    }

    // 4. Alocar e ler os dados da imagem (pixels)
    int** imagem = alocar_imagem(*ponteiro_altura, *ponteiro_largura);
    if (imagem == NULL) {
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < *ponteiro_altura; i++) {
        for (int j = 0; j < *ponteiro_largura; j++) {
            // Tenta ler o próximo número (pixel)
            if (fscanf(arquivo, "%d", &imagem[i][j]) != 1) {
                printf("Erro: Fim inesperado do arquivo ao ler os pixels.\n");
                liberar_imagem(imagem, *ponteiro_altura); // Libera o que já alocou
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo); // Fecha o arquivo
    printf("Arquivo '%s' lido com sucesso (%dx%d).\n", nome_arquivo, *ponteiro_largura, *ponteiro_altura);
    return imagem;
}


/**
 * Verifica se uma sub-imagem (bloco) é homogênea (todos os pixels da mesma cor).
 * Parâmetros:
 * imagem: A matriz da imagem completa.
 * linha_inicio, coluna_inicio: Coordenadas do canto superior esquerdo do bloco.
 * altura_bloco, largura_bloco: Dimensões do bloco.
 * Retorna:
 * COR_BRANCO (0) se todos forem brancos.
 * COR_PRETO (1) se todos forem pretos.
 * COR_MISTA (-1) se houver ambas as cores.
 */
int verificar_se_e_homogenea(int** imagem, int linha_inicio, int coluna_inicio, int altura_bloco, int largura_bloco) {
    if (altura_bloco <= 0 || largura_bloco <= 0) return COR_MISTA; // Caso inválido

    // Pega o primeiro pixel do bloco como referência
    int cor_base = imagem[linha_inicio][coluna_inicio];
    
    // Percorre todos os pixels do bloco
    for (int i = linha_inicio; i < linha_inicio + altura_bloco; i++) {
        for (int j = coluna_inicio; j < coluna_inicio + largura_bloco; j++) {
            // Se achar um pixel diferente da cor base...
            if (imagem[i][j] != cor_base) {
                return COR_MISTA; // ...retorna que é mista.
            }
        }
    }
    
    return cor_base; // Se saiu do loop, todas as cores são iguais à cor_base
}


/**
 * Função RECURSIVA principal para codificar a imagem.
 * Imprime o código (P, B ou X...) diretamente na tela.
 * Parâmetros:
 * imagem: A matriz da imagem completa.
 * linha_inicio, coluna_inicio: Coordenadas do canto superior esquerdo do bloco atual.
 * altura_bloco, largura_bloco: Dimensões do bloco atual.
 000000*/
void codificar_imagem(int** imagem, int linha_inicio, int coluna_inicio, int altura_bloco, int largura_bloco) {
    
    // 1. Verifica se o bloco atual é homogêneo
    int cor = verificar_se_e_homogenea(imagem, linha_inicio, coluna_inicio, altura_bloco, largura_bloco);

    // CASO BASE 1 (CONDIÇÃO DE PARADA)
    if (cor == COR_BRANCO) {
        // Regra 1: Imagem toda branca
        printf("B");
        return; // Para a recursão para este ramo
    }
    
    // CASO BASE 2 (CONDIÇÃO DE PARADA)
    if (cor == COR_PRETO) {
        // Regra 1: Imagem toda preta
        printf("P");
        return; // Para a recursão para este ramo
    }

    // PASSO RECURSIVO (DIVISÃO)
    // Regra 2: Imagem misturada
    printf("X");

    // 2. Calcular os pontos de corte (divisão)
    // A divisão de inteiros em C segue as regras do enunciado:
    // "superiores terão uma linha a mais" -> (altura_bloco + 1) / 2
    // "esquerdo terão uma coluna a mais" -> (largura_bloco + 1) / 2
    
    int altura_superior = (altura_bloco + 1) / 2;
    int altura_inferior = altura_bloco / 2;
    int largura_esquerda = (largura_bloco + 1) / 2;
    int largura_direita = largura_bloco / 2;

    // 3. Chamar recursivamente para os 4 quadrantes
    // É importante verificar se a altura/largura do quadrante é > 0
    // Isso trata o caso de imagens com 1 linha ou 1 coluna (Figura 5)

    // Quadrante 1: Superior Esquerdo
    if (altura_superior > 0 && largura_esquerda > 0) {
        codificar_imagem(imagem, 
                         linha_inicio, 
                         coluna_inicio, 
                         altura_superior, 
                         largura_esquerda);
    }
    
    // Quadrante 2: Superior Direito
    if (altura_superior > 0 && largura_direita > 0) {
        codificar_imagem(imagem, 
                         linha_inicio, 
                         coluna_inicio + largura_esquerda, // Desloca coluna
                         altura_superior, 
                         largura_direita);
    }

    // Quadrante 3: Inferior Esquerdo
    if (altura_inferior > 0 && largura_esquerda > 0) {
        codificar_imagem(imagem, 
                         linha_inicio + altura_superior, // Desloca linha
                         coluna_inicio, 
                         altura_inferior, 
                         largura_esquerda);
    }

    // Quadrante 4: Inferior Direito
    if (altura_inferior > 0 && largura_direita > 0) {
        codificar_imagem(imagem, 
                         linha_inicio + altura_superior, // Desloca linha
                         coluna_inicio + largura_esquerda, // Desloca coluna
                         altura_inferior, 
                         largura_direita);
    }
}