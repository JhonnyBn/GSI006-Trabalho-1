/*

    Programa dedicado a mostrar tabelas de Pecas x Modelos de carros

    Feito por:

    Caliton Marcos Goncalves Junior - 11711BSI261
    Hugo Sousa Nasciutti - 11621BSI260
    Joao Barbosa de Souza Neto - 1711BSI212

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXNAMESIZE 30

// --- Structs ---

struct matrix
{
    int rows;
    int cols;
    int **matrix;
    char **rowsNames;
    char **colsNames;
};
typedef struct matrix MATRIX;

// --- Prototypes ----

int argsOk(int argc, char *argv[]);
void carsbytype(MATRIX *modelos);
void carsbyversion(MATRIX *modelos);
MATRIX * getContent(char *fileName);
int getArgs(MATRIX *m, char *string, int strsize);
void getElements(MATRIX *m, char *fileName);
int getFileSize(char *fileName);
int** matrixalloc(int rows, int cols);
void partsbyversion(MATRIX *pecas);
int partsperversion(MATRIX *modelos, int version, MATRIX *pecas, int part);
void printFile (MATRIX *m);
int sum (int* v, int size);
char** svalloc(int rows, int cols);
void totalpartsbycarversion(MATRIX *modelos, MATRIX *pecas);

// --- Main ---

int main(int argc, char *argv[])
{
    MATRIX *modelos; // Guarda a matriz de modelos
    MATRIX *pecas; // Guarda a matriz de peças

    int mdl = 0; // Auxiliar para guardar a posição da matriz de modelos de carro em argv
    int pcs = 0; // Auxiliar para guardar a posição da matriz de peças de carro em argv

    if (!argsOk(argc, argv))
        return 1;

    if (!strcmp(argv[1],"-mdl"))
    {
        mdl = 2;
        pcs = 4;
    }
    else
    {
        mdl = 4;
        pcs = 2;
    }
    modelos = getContent(argv[mdl]);
    pecas = getContent(argv[pcs]);

    printFile(modelos);
    printf("\n");
    printFile(pecas);
    printf("\n");

    carsbytype(modelos);
    carsbyversion(modelos);
    partsbyversion(pecas);
    totalpartsbycarversion(modelos,pecas);

    return 0;
}

// --- Functions ---

/*
    argsOk têm como parâmetro argc e argv da main,
    Sua função é verificar se os argumentos estão de acordo com a proposta do programa
    retornando 1 se sim,
    e se não, exibe erro e retorna 0.
*/
int
argsOk(int argc, char *argv[])
{
    if (argc < 5)
    {
        fprintf(stderr, "%s: missing arguments.\n", argv[0]);
        return 0;
    }
    if (argc > 5)
    {
        fprintf(stderr, "%s: too many arguments.\n", argv[0]);
        return 0;
    }
    if ( ( (!strcmp(argv[1],"-mdl") ) && (!strcmp(argv[3],"-pcs") ) ) || ( (!strcmp(argv[1],"-pcs") ) && (!strcmp(argv[3],"-mdl") ) ) )
    {
        return 1;
    }
    else
    {
        fprintf(stderr, "%s and/or %s: Invalid tags.\n", argv[1], argv[3]);
        return 0;
    }
}

/*
    carsbytype têm como parâmetro duas MATRIX,
    Sua função é mostrar quantos veículos de um determinado tipo foram construídos, por tipo e total.
*/
void
carsbytype(MATRIX *modelos)
{
    int i;
    int* v;
    v = (int*)malloc(modelos->rows*sizeof(int));
    if (v == NULL)
    {
        fprintf(stderr, "Erro ao alocar vetor para soma.\n");
    }

    printf("total cars by type\n------------------\n");
    for (i = 0; i < modelos->rows; i++)
    {
        v[i] = 0;
        v[i] = sum(modelos->matrix[i], modelos->cols);
        printf ("version %s = \t %d\n", modelos->rowsNames[i], v[i]);
    }
    printf("\n");
    printf("Total: \t %d", sum(v, modelos->rows));
    printf("\n\n");
}

/*
    carsbyversion têm como parâmetro duas MATRIX,
    Sua função é mostrar quantos veículos de uma determinada versão foram construídos, por versão e total.
*/
void
carsbyversion(MATRIX *modelos)
{
    int i, j;
    int* v;
    v = (int*)malloc(modelos->cols*sizeof(int));
    if (v == NULL)
    {
        fprintf(stderr, "Erro ao alocar vetor para soma.\n");
    }

    printf("total cars by version\n---------------------\n");
    for (i = 0; i < modelos->cols; i++)
    {
        v[i] = 0;
        for (j = 0; j < modelos->rows; j++)
        {
            v[i] += modelos->matrix[j][i];
        }
        printf ("version %s = \t %d\n", modelos->colsNames[i], v[i]);
    }
    printf("\n");
    printf("Total: \t %d", sum(v, modelos->cols));
    printf("\n\n");
}

/*
    getArgs têm como parâmetro um ponteiro de struct MATRIX, um vetor de char e seu tamanho,
    Sua função é contar a quantidade de palavras num arquivo armazenado num vetor de char com pelo menos 2 \n's,
    e então verifica se o resto do vetor contém números ou \n's e \0's, retornando 1 se esta verificação der certo, ou 0 se não.
*/
int
getArgs(MATRIX *m, char *string, int strsize)
{
    int i = 0;
    int args = 0; // Guarda a quantidade de palavras

    // Percorre a primeira linha do vetor
    while(string[i] != '\n')
    {
        if(string[i] == ' ')
            args++; // Conta a quantidade de palavras
        i++;
    }
    m->rows = args+1; // Guarda a quantidade de linhas da matriz
    args = 0;
    i++;
    // Percorre a segunda linha do vetor
    while(string[i] != '\n') {
        if(string[i] == ' ') {
            args++; // Conta a quantidade de palavras
        }
        i++;
    }
    m->cols = args+1; // Guarda a quantidade de colunas da matriz

    // Verifica todo o resto da string
    for (; i < strsize; i++)
    {
        if ( ( !isdigit(string[i]) ) && (string[i] != ' ') && (string[i] != '\n') && (string[i] != '\0') ) // Caso encontre um elemento inválido para calcular matrizes, retorna 0
            return 0;
    }

    // Em caso de matriz válida, retorna 1
    return 1;
}

/*
    getContent têm como parâmetro o nome de um arquivo,
    Sua funnção é carregar uma struct MATRIX,
    retornando o endereço do MATRIX,
    ou NULL se ocorreu erro
*/
MATRIX*
getContent(char *fileName)
{
    int size = getFileSize(fileName);
    char *buffer = (char *) malloc(size * sizeof(char));
    MATRIX* m = (MATRIX*)malloc(sizeof(MATRIX));
    FILE *fp = fopen(fileName, "r");
    if(!fp) {
        fprintf(stderr, "%s: file not found.\n", fileName);
        return NULL;
    }
    fread(buffer, sizeof(char), size, fp);
    buffer[size - 1] = '\0';

    if (!getArgs(m, buffer, size))
    {
        fprintf(stderr,"Matriz com caracteres invalidos.\n");
        return NULL;
    }
    m->rowsNames = svalloc(m->rows, MAXNAMESIZE);
    if (m->rowsNames == NULL) {
        fprintf (stderr, "Erro ao alocar a matriz.\n");
        return NULL;
    }
    m->colsNames = svalloc(m->cols, MAXNAMESIZE);
    if (m->colsNames == NULL) {
        fprintf (stderr, "Erro ao alocar a matriz.\n");
        return NULL;
    }
    m->matrix = matrixalloc(m->rows, m->cols);
    if (m->matrix == NULL) {
        fprintf (stderr, "Erro ao alocar a matriz.\n");
        return NULL;
    }
    getElements(m, fileName);

    fclose(fp);
    return m;
}

/*
    getElements têm como parâmetro um ponteiro de struct MATRIX e o nome de um arquivo,
    Sua função é carregar os elementos da struct
*/
void
getElements(MATRIX *m, char *fileName)
{
    int i, j;
    FILE *fp = fopen (fileName, "r");

    for (i = 0; i < m->rows; i++)
        fscanf (fp, "%s", m->rowsNames[i]);
    for (i = 0; i < m->cols; i++)
        fscanf (fp, "%s", m->colsNames[i]);
    for (i = 0; i < m->rows; i++)
        for (j = 0; j < m->cols; j++)
            fscanf (fp, "%d ", &m->matrix[i][j]);

    fclose (fp);
}

/*
    getFileSize tem como parâmetro o nome de um arquivo,
    Sua função é retornar a quantidade de caracteres do mesmo,
    retornando 0 e exibindo uma mensagem em caso de erro.
*/
int
getFileSize(char *fileName)
{
    int size = 0;
    FILE *fp = fopen(fileName, "r");
    if(!fp) {
        fprintf(stderr, "%s: file not found.\n", fileName);
        return 0;
    }

    while(fgetc(fp) != EOF) {
        size++;
    }
    fclose(fp);
    return size;
}

/*
    matrixalloc recebe a quantidade de Linhas e Colunas como Parâmetro,
    e alloca dinamicamente uma matriz de números inteiros,
    retornando a posição dessa matriz, ou NULL em caso de erro, junto de uma mensagem.
*/
int**
matrixalloc(int rows, int cols)
{

    int i,j; //Variáveis Auxiliares

    int **m = (int**)malloc(rows * sizeof(int*)); //Aloca um Vetor de Ponteiros
    if (m == NULL)
    {
        fprintf(stderr, "Erro na alocacao da matriz.\n");
        return NULL;
    }
    for (i = 0; i < rows; i++) //Percorre as linhas do Vetor de Ponteiros
    {
        m[i] = (int*) malloc(cols * sizeof(int)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
        for (j = 0; j < cols; j++) //Percorre o Vetor de Inteiros atual.
        {
            m[i][j] = 0; //Inicializa com 0.
        }
    }
    return m; //Retorna o Ponteiro para a Matriz Alocada
}

/*
    partsbyversion têm como parâmetro duas MATRIX,
    Sua função é mostrar quantas peças foram utilizadas na produção de um determinado tipo de veículo, independente da versão, por tipo e total.
*/
void
partsbyversion(MATRIX *pecas)
{
    int i, j;
    int* total;
    int* v;
    v = (int*)malloc(pecas->rows*sizeof(int));
    if (v == NULL)
    {
        fprintf(stderr, "Erro ao alocar vetor para soma.\n");
    }
    total = (int*)malloc(pecas->rows*sizeof(int));
    if (total == NULL)
    {
        fprintf(stderr, "Erro ao alocar vetor para total de pecas.\n");
    }

    printf("total parts by version\n----------------------\n");
    for (i = 0; i < pecas->cols; i++)
    {
        total[i] = 0;
        printf("car type %s\n", pecas->colsNames[i]);
        for (j = 0; j < pecas->rows; j++)
        {
            v[j] = 0;
            v[j] = pecas->matrix[j][i];
            printf ("part %s = %d\n", pecas->rowsNames[j], v[j]);
        }
        total[i] = sum(v, pecas->rows);
        printf("TOTAL = %d", total[i]);
        printf("\n\n");
    }
    printf("TOTAL OF PARTS = %d\n\n", sum(total, pecas->cols));
}

/*
    printFile recebe como parâmetro uma struct MATRIX,
    Sua função é imprimir todos os dados dessa struct.
*/
void
printFile (MATRIX *m)
{
    int i, j;
    for (i = 0; i < m->rows; i++)
        printf("%s ", m->rowsNames[i]);
    printf ("\n");
    for (i = 0; i < m->cols; i++)
        printf("%s ", m->colsNames[i]);
    printf ("\n");
    for (i = 0; i < m->rows; i++)
    {
        for (j = 0; j < m->cols; j++)
            printf("%d ", m->matrix[i][j]);
        printf ("\n");
    }
}

/*
    sum recebe um vetor e seu tamanho,
    ele retorna a soma dos elementos do vetor
*/
int
sum(int* v, int size)
{
    int i;
    int s = 0; // Variável para a soma

    for (i = 0; i < size; i++)
        s += v[i]; // Soma o vetor
    return s;
}

/*
    svalloc recebe a quantidade de strings e tamanho máximo de letras por string como Parâmetro,
    e alloca dinamicamente uma matriz de chars para criar um "vetor de strings",
    retornando a posição dessa matriz, ou NULL em caso de erro, junto de uma mensagem.
*/
char**
svalloc(int rows, int cols) //Recebe a quantidade de Linhas e Colunas como Parâmetro
{

    int i,j; //Variáveis Auxiliares

    char **m = (char**)malloc(rows * sizeof(char*)); //Aloca um Vetor de Ponteiros
    if (m == NULL)
    {
        fprintf(stderr, "Erro na alocacao da matriz.\n");
        return NULL;
    }
    for (i = 0; i < rows; i++) //Percorre as rows do Vetor de Ponteiros
    {
        m[i] = (char*) malloc(cols * sizeof(char)); //Aloca um Vetor de Chars para cada posição do Vetor de Ponteiros.
        for (j = 0; j < cols; j++) //Percorre o Vetor de Chars atual.
        {
            m[i][j] = '\0'; //Inicializa com '\0'.
        }
    }
    return m; //Retorna o Ponteiro para a Matriz Alocada
}

/*
    totalpartsbycarversion têm como parâmetro duas MATRIX,
    Sua função é mostrar quantas peças foram utilizadas na produção de uma determinada versão, independente do tipo, por versão e total.
*/
void
totalpartsbycarversion(MATRIX *modelos, MATRIX *pecas)
{
    int i, j, k;
    int* total;
    int* v;
    v = (int*)malloc(pecas->rows*sizeof(int));
    if (v == NULL)
    {
        fprintf(stderr, "Erro ao alocar vetor para soma.\n");
    }
    total = (int*)malloc(pecas->rows*sizeof(int));
    if (total == NULL)
    {
        fprintf(stderr, "Erro ao alocar vetor para total de pecas.\n");
    }

    printf("total parts by car version\n---------------------------\n");
    for (i = 0; i < modelos->cols; i++)
    {
        total[i] = 0;
        printf("car version %s\n", modelos->colsNames[i]);
        for (j = 0; j < pecas->rows; j++)
        {
            v[j] = 0;
            for (k = 0; k < pecas->cols; k++)
            {
                v[j] += (modelos->matrix[k][i]) * (pecas->matrix[j][k]);
            }
            printf ("part %s = %d\n", pecas->rowsNames[j], v[j]);
        }
        total[i] = sum(v, pecas->rows);
        printf("TOTAL = %d", total[i]);
        printf("\n\n");
    }
    printf("TOTAL OF PARTS = %d\n\n", sum(total, modelos->cols));
}

