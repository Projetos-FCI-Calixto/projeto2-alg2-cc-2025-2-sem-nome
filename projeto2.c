#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições de constantes
#define MAX_LARGURA 1024
#define MAX_ALTURA 768

// Definição de tipo para a imagem binária
typedef struct {
    int largura;               // Número de colunas
    int altura;                // Número de linhas
    int pixels[MAX_ALTURA][MAX_LARGURA]; // Matriz de pixels: 0 = branco, 1 = preto
} Imagem;

// ----------------------------
// Funções de leitura da imagem
// ----------------------------

// Função para ler imagem a partir de arquivo PBM
Imagem lerImagemArquivo(const char* nomeArquivo) {
    Imagem img;
    // 1. Abrir o arquivo em modo leitura
    // 2. Verificar se o arquivo abriu corretamente
    // 3. Ler o "magic number" (P1) e validar
    // 4. Ignorar linhas de comentário (começam com #)
    // 5. Ler largura e altura da imagem
    // 6. Ler cada valor de pixel (0 ou 1) e armazenar na matriz img.pixels
    // 7. Retornar a estrutura Imagem preenchida
}

// Função para ler imagem manualmente via teclado
Imagem lerImagemManual() {
    Imagem img;
    // 1. Solicitar ao usuário a largura e altura da imagem
    // 2. Validar os valores informados (não ultrapassar MAX_LARGURA e MAX_ALTURA)
    // 3. Ler cada pixel individualmente (0 ou 1) do usuário
    // 4. Armazenar na matriz img.pixels
    // 5. Retornar a estrutura Imagem preenchida
}

// ----------------------------
// Funções de codificação
// ----------------------------

// Função para verificar se uma subimagem é homogênea
char verificarHomogeneidade(Imagem img, int linhaInicio, int linhaFim, int colInicio, int colFim) {
    // 1. Percorrer os pixels da subimagem delimitada por linhaInicio/linhaFim e colInicio/colFim
    // 2. Comparar todos os pixels com o primeiro pixel da subimagem
    // 3. Se todos iguais:
    //      - Retornar 'B' para branco (0)
    //      - Retornar 'P' para preto (1)
    // 4. Se houver diferença entre pixels, retornar 'X'
}

// Função recursiva para codificar uma subimagem
void codificarSubimagem(Imagem img, int linhaInicio, int linhaFim, int colInicio, int colFim, char* codigo, int* pos) {
    // 1. Verificar homogeneidade da subimagem usando verificarHomogeneidade()
    // 2. Se homogênea:
    //      - Adicionar 'B' ou 'P' no vetor codigo[pos]
    //      - Incrementar pos
    // 3. Se não homogênea:
    //      - Adicionar 'X' no vetor codigo[pos]
    //      - Incrementar pos
    //      - Calcular ponto médio para cortes horizontal e vertical
    //      - Ajustar cortes para lidar com linhas/colunas ímpares
    //      - Chamar codificarSubimagem recursivamente para os 4 quadrantes
    //        na ordem: superior esquerda, superior direita, inferior esquerda, inferior direita
}

// Função principal de codificação
char* codificarImagem(Imagem img) {
    // 1. Criar vetor de caracteres para armazenar o código final
    // 2. Inicializar posição com 0
    // 3. Chamar codificarSubimagem para toda a imagem
    // 4. Retornar o código completo
}

// ----------------------------
// Função para exibir ajuda
// ----------------------------
void mostrarAjuda() {
    printf("Uso: ImageEncoder [-? | -m | -f ARQ]\n");
    printf("Codifica imagens binárias dadas em arquivos PBM ou por dados informados manualmente.\n");
    printf("Argumentos:\n");
    printf("-?, --help    : apresenta esta ajuda\n");
    printf("-m, --manual  : ativa modo manual de entrada da imagem\n");
    printf("-f, --file ARQ: considera a imagem representada no arquivo PBM ARQ\n");
}

// ----------------------------
// Função main
// ----------------------------
int main(int argc, char* argv[]) {
    // 1. Verificar se nenhum argumento foi informado
    //      - Chamar mostrarAjuda() e encerrar
    // 2. Verificar o tipo de entrada:
    //      - "-m" ou "--manual" -> lerImagemManual()
    //      - "-f" ou "--file"   -> lerImagemArquivo(nomeArquivo)
    //      - "-?" ou "--help"   -> mostrarAjuda()
    // 3. Após obter a imagem, chamar codificarImagem()
    // 4. Exibir o código gerado
    // 5. Finalizar o programa
}