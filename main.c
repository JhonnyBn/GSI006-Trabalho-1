/*
	Programa dedicado a mostrar tabelas de Pe�as x Modelos de carros
	
	Feito por:
	
	Caliton Marcos Goncalves Junior - 11711BSI261
	Hugo Sousa Nasciutti - 11621BSI260
	Joao Barbosa de Souza Neto - 1711BSI212
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLETRAS 30

struct matriz
{
    int linhas;
    int colunas;
    char** cabecalhoum;
    char** cabecalhodois;
    int** matriz;
};
typedef struct matriz Matriz;

// Prot�tipos

int** alocarMatriz(int Linhas, int Colunas);
char** alocarMatrizChar(int Linhas, int Colunas);
int argsOk(int argc, char *argv[]);
void carsbytype(Matriz *modelos);
void carsbyversion(Matriz *modelos);
int compatibleMatrix(Matriz *modelos, Matriz *pecas);
void freeMatriz(Matriz *m);
Matriz* loadMatriz(char *arquivo);
void partsbyversion(Matriz *pecas);
void printMatriz(Matriz *m);
int sum (int* v, int size);
int validchar(int c);
void totalpartsbycarversion(Matriz *modelos, Matriz *pecas);

// Main

int
main (int argc, char *argv[])
{
    Matriz *modelos; // Guarda a matriz de modelos
    Matriz *pecas; // Guarda a matriz de pe�as

    int mdl = 0; // Auxiliar para guardar a posi��o da matriz de modelos de carro em argv
    int pcs = 0; // Auxiliar para guardar a posi��o da matriz de pe�as de carro em argv

    if (!argsOk(argc, argv)) // Se houve erro nos argumentos, encerra o programa retornando 1
        return 1;

    if (!strcmp(argv[1],"-mdl")) // Checa a posi��o dos arquivos para carregar a Matriz
    {
        mdl = 2;
        pcs = 4;
    }
    else
    {
        mdl = 4;
        pcs = 2;
    }
	
    modelos = loadMatriz(argv[mdl]); // Carrega a Matriz de modelos
	pecas = loadMatriz(argv[pcs]); // Carrega a Matriz de pe�as
	
	if (modelos == NULL || pecas == NULL) // Se houve erro no carregamento, encerra o programa retornando 1
		return 1;
	
	if (!compatibleMatrix(modelos, pecas)) // Se as matrizes n�o forem compat�veis, encerra o programa retornando 1
		return 1;
	
    carsbytype(modelos); // Problema A
    carsbyversion(modelos); // Problema B
    partsbyversion(pecas); // Problema C
    totalpartsbycarversion(modelos, pecas); // Problema D
	
	freeMatriz(modelos);
	freeMatriz(pecas);

    return 0;
}

// --- Functions ---

/*
    alocarMatriz recebe a quantidade de Linhas e Colunas como Par�metro,
    e alloca dinamicamente uma matriz de n�meros inteiros,
    retornando a posi��o dessa matriz, ou NULL em caso de erro, junto de uma mensagem.
*/
int** alocarMatriz(int Linhas,int Colunas)  // Recebe a quantidade de Linhas e Colunas como Par�metro
{

	int i,j; // Vari�veis Auxiliares

	int **m = (int**)malloc(Linhas * sizeof(int*)); // Aloca um Vetor de Ponteiros
	if (m == NULL) // Retorna NULL se houve erro na aloca��o
		return NULL;

	for (i = 0; i < Linhas; i++) // Percorre as linhas do Vetor de Ponteiros
	{
		m[i] = (int*) malloc(Colunas * sizeof(int)); // Aloca um Vetor de Inteiros para cada posi��o do Vetor de Ponteiros
		if (m[i] == NULL) // Em caso de erro de aloca��o, libera a matriz e retorna NULL
		{
			while (--i >= 0)
				free(m[i]);
			free(m);
			return NULL;
		}
		for (j = 0; j < Colunas; j++) // Percorre o Vetor de Inteiros atual
		{
			m[i][j] = 0; // Inicializa com 0
		}
	}
	return m; //Retorna o Ponteiro para a Matriz Alocada
}

/*
    alocarMatrizChar recebe a quantidade de strings e tamanho m�ximo de letras por string como Par�metro,
    e alloca dinamicamente uma matriz de chars para criar um "vetor de strings",
    retornando a posi��o dessa matriz, ou NULL em caso de erro, junto de uma mensagem.
*/
char** alocarMatrizChar(int Linhas,int Colunas) // Recebe a quantidade de Linhas e Colunas como Par�metro
{

	int i,j; // Vari�veis Auxiliares

	char **m = (char**)malloc(Linhas * sizeof(char*)); // Aloca um Vetor de Ponteiros
	if (m == NULL) // Retorna NULL se houve erro na aloca��o
		return NULL;

	for (i = 0; i < Linhas; i++) // Percorre as linhas do Vetor de Ponteiros
	{
		m[i] = (char*) malloc(Colunas * sizeof(char)); // Aloca um Vetor para cada posi��o do Vetor de Ponteiros.
		if (m[i] == NULL) // Em caso de erro de aloca��o, libera a matriz e retorna NULL
		{
			while (--i >= 0)
				free(m[i]);
			free(m);
			return NULL;
		}
		for (j = 0; j < Colunas; j++) // Percorre o Vetor atual.
		{
			m[i][j] = '\0'; // Inicializa com \0.
		}
	}
	return m; // Retorna o Ponteiro para a Matriz Alocada
}

/*
    argsOk t�m como par�metro argc e argv da main,
    Sua fun��o � verificar se os argumentos est�o de acordo com a proposta do programa
    retornando 1 se sim,
    e se n�o, exibe erro e retorna 0.
*/
int
argsOk(int argc, char *argv[])
{
    if (argc < 5) // Checa se o programa recebeu poucos argumentos
    {
        fprintf(stderr, "%s: missing arguments.\n", argv[0]);
        return 0;
    }
    if (argc > 5) // Checa se o programa recebeu argumentos demais
    {
        fprintf(stderr, "%s: too many arguments.\n", argv[0]);
        return 0;
    }
    if ( ( (!strcmp(argv[1],"-mdl")) && (!strcmp(argv[3],"-pcs")) ) || ( (!strcmp(argv[1],"-pcs")) && (!strcmp(argv[3],"-mdl")) ) ) // Verifica se os par�metros s�o v�lidos
    {
        return 1;
    }
    else if ( (!strcmp(argv[1],"-mdl")) || (!strcmp(argv[1],"-pcs")) ) // Caso o primeiro par�metro seja v�lido e o segundo inv�lido
	{
		fprintf(stderr, "%s: invalid parameter\n", argv[3]);
        return 0;
    }
	else // Caso o primeiro par�metro seja inv�lido
	{
		fprintf(stderr, "%s: invalid parameter\n", argv[1]);
		return 0;
	}
}

/*
    carsbytype t�m como par�metro duas Matriz,
    Sua fun��o � mostrar quantos ve�culos de um determinado tipo foram constru�dos, por tipo e total.
*/
void
carsbytype(Matriz *modelos)
{
    int i; // Vari�vel auxiliar
    int* v; // Ponteiro para armazenar o numero de ve�culos
    
	v = (int*)malloc(modelos->linhas*sizeof(int));
	
    if (v == NULL)
    {
        fprintf(stderr, "Out of memory\n");
		exit(1);
    }
	
	printf("\na) quantos veiculos de um determinado tipo foram construidos? Por tipo e total\n\n");
    printf("total cars by type\n------------------\n");
    for (i = 0; i < modelos->linhas; i++)
    {
        v[i] = 0;
        v[i] = sum(modelos->matriz[i], modelos->colunas);
        printf ("version %s = \t %d\n", modelos->cabecalhoum[i], v[i]);
    }
    printf("TOTAL of cars = %d\n", sum(v, modelos->linhas));
}

/*
    carsbyversion t�m como par�metro duas Matriz,
    Sua fun��o � mostrar quantos ve�culos de uma determinada vers�o foram constru�dos, por vers�o e total.
*/
void
carsbyversion(Matriz *modelos)
{
    int i, j; // Vari�veis auxiliares
    int* v; // Ponteiro para armazenar o numero de ve�culos
	
    v = (int*)malloc(modelos->colunas*sizeof(int));
    
	if (v == NULL)
    {
        fprintf(stderr, "Out of memory\n");
		exit(1);
    }
	
	printf("\nb) quantos veiculos de uma determinada versao foram construidos? Por versao e total.\n\n");
    printf("total cars by version\n---------------------\n");
    for (i = 0; i < modelos->colunas; i++)
    {
        v[i] = 0;
        for (j = 0; j < modelos->linhas; j++)
        {
            v[i] += modelos->matriz[j][i];
        }
        printf ("version %s = \t %d\n", modelos->cabecalhodois[i], v[i]);
    }
    printf("TOTAL of cars = %d\n", sum(v, modelos->colunas));
}

/*
	compatibleMatrix recebe duas Matriz* como argumento,
	Sua fun��o � retornar 1 se as matrizes s�o compat�veis ou 0 em caso contr�rio
*/
int
compatibleMatrix(Matriz *modelos, Matriz *pecas)
{
	int i;
	if (modelos->linhas != pecas->colunas)
	{
		fprintf(stderr, "incompatible matrixes: different number of car types\n");
		return 0;
	}
	for (i = 0; i < modelos->linhas; i++)
		if (strcmp(modelos->cabecalhoum[i], pecas->cabecalhodois[i]) != 0)
		{
			fprintf(stderr, "incompatible matrixes: car type #%d has different names: '%s' and '%s'\n", i+1, modelos->cabecalhoum[i], pecas->cabecalhodois[i]);
			return 0;
		}
	
	return 1;
}

/*
	freeMatriz recebe um ponteiro de Matrix como argumento,
	e libera a mem�ria dos membros de sua struct
*/
void freeMatriz(Matriz* m) {
	int i;
	if (m->cabecalhoum != NULL) // Se cabecalhoum estiver carregado
	{
		for (i = 0; i < m->linhas; i++) // Libera cada string do cabecalho
			free(m->cabecalhoum[i]);
		free(m->cabecalhoum); // Libera o cabecalho
	}
	if (m->cabecalhodois != NULL) // Se cabecalhodois estiver carregado
	{
		for (i = 0; i < m->colunas; i++) // Libera cada string do cabecalho
			free(m->cabecalhodois[i]);
		free(m->cabecalhodois); // Libera o cabecalho
	}
	if (m->matriz != NULL) // Se a matriz estiver carregada
	{
		for (i = 0; i < m->linhas; i++) // Libera cada linha da matriz
			free(m->matriz[i]);
		free(m->matriz); // Libera a matriz
	}
	free(m); // Libera a struct
}

/*
	loadMatrix t�m como par�metro um stringa que corresponde ao nome de um arquivo,
	Sua fun��o � retornar uma Matrix com todos os dados preenchidos.
*/
Matriz*
loadMatriz (char *arquivo)
{
    int i = 0, j = 0; // Vari�veis auxiliares
    char aux = 0; // Auxiliar para ler o arquivo
    char palavras = 0; // Auxiliar para contar a quantidade de palavras nos cabe�alhos
	FILE *fp = fopen(arquivo,"r"); // Arquivo que ser� lido
	Matriz* m = (Matriz*)malloc(sizeof(Matriz)); // Matriz a ser preenchida
	
	if (fp == NULL) // Caso o arquivo n�o seja encontrado
	{
		fprintf(stderr, "%s: File not found", arquivo);
		free(m);
		return NULL;
	}
	if (m == NULL) // Caso o malloc Matriz falhe
	{
		fprintf(stderr,"Out of memory\n");
		fclose(fp);
		return NULL;
	}
	
    while(aux != '\n') // Percorre a primeira linha
    {
        fscanf(fp, "%c", &aux);
        if (aux == ' ')
            palavras++; // Conta quantas palavras
    }
    m->linhas = palavras+1;

    fscanf(fp, "%c", &aux); // Passa para o pr�ximo caractere ap�s o '\n' da linha anterior

    palavras = 0; // Reseta o contador de palavras
    while(aux != '\n')
    {
        fscanf(fp, "%c", &aux);
        if (aux == ' ')
            palavras++; // Conta quantas palavras h� na segunda linha
    }
    m->colunas = palavras+1;
	
	while (fscanf(fp, "%c", &aux) == 1) // Percorre o resto do arquivo verificando a matriz
	{
		if (!validchar(aux)) // Se um caractere n�o for v�lido para matrizes retorna NULL
		{
			fprintf(stderr, "%s: invalid character '%c' in matrix line %d column %d\n", arquivo, aux, i+2, j);
			fclose(fp);
			free(m);
			return NULL;
		}
		if (aux == '\n') i++;
		j++;
	}
    fclose(fp);

    m->cabecalhoum = alocarMatrizChar(m->linhas, MAXLETRAS); // Aloca dinamicamente um vetor de strings para o primeiro cabe�alho
    m->cabecalhodois = alocarMatrizChar(m->colunas, MAXLETRAS); // Aloca dinamicamente um vetor de strings para o segundo cabe�alho
    m->matriz = alocarMatriz(m->linhas, m->colunas); // Aloca dinamicamente a matriz de inteiros
	
	if (m->cabecalhoum == NULL || m->cabecalhodois == NULL || m->matriz == NULL) // Em caso de erro nas aloca��es, retorna NULL
	{
		fprintf(stderr,"Out of memory\n");
		freeMatriz(m);
		return NULL;
	}

    fp = fopen(arquivo, "r"); // Releitura do arquivo, agora armazenando os valores para a Matriz

    for (i = 0; i < m->linhas; i++)
        fscanf(fp, "%s", m->cabecalhoum[i]);

    for (i = 0; i < m->colunas; i++)
        fscanf (fp, "%s", m->cabecalhodois[i]);

    for (i = 0; i < m->linhas; i++)
        for (j = 0; j < m->colunas; j++)
            fscanf(fp, "%d", &m->matriz[i][j]);

    fclose(fp);

    return m;
}

/*
    partsbyversion t�m como par�metro duas Matriz,
    Sua fun��o � mostrar quantas pe�as foram utilizadas na produ��o de um determinado tipo de ve�culo, independente da vers�o, por tipo e total.
*/
void
partsbyversion(Matriz *pecas)
{
    int i, j; // Vari�veis auxiliares
	int* v; // Ponteiro para armazenar as partes
    int* total; // Ponteiro para armazenar o total de partes
	
    v = (int*)malloc(pecas->linhas*sizeof(int));
	total = (int*)malloc(pecas->linhas*sizeof(int));
    
	if (v == NULL || total == NULL)
    {
        fprintf(stderr, "Out of memory\n");
		free(v);
		free(total);
		exit(1);
    }
	
	printf("\nc) quantas pecas sao utilizadas na producao de um determinado tipo de veiculo, independente da versao? Por tipo e total.\n\n");
    printf("total parts by version\n----------------------\n");
    for (i = 0; i < pecas->colunas; i++)
    {
        total[i] = 0;
        printf("car type %s\n", pecas->cabecalhodois[i]);
        for (j = 0; j < pecas->linhas; j++)
        {
            v[j] = 0;
            v[j] = pecas->matriz[j][i];
            printf ("part %s = %d\n", pecas->cabecalhoum[j], v[j]);
        }
        total[i] = sum(v, pecas->linhas);
        printf("TOTAL = %d\n\n", total[i]);
    }
    printf("TOTAL OF PARTS = %d\n", sum(total, pecas->colunas));
}

/*
    sum recebe um vetor e seu tamanho,
    ele retorna a soma dos elementos do vetor
*/
int
sum(int* v, int size)
{
    int i;
    int s = 0; // Vari�vel para a soma

    for (i = 0; i < size; i++)
        s += v[i]; // Soma o vetor
    return s;
}

/*
    totalpartsbycarversion t�m como par�metro duas Matriz,
    Sua fun��o � mostrar quantas pe�as foram utilizadas na produ��o de uma determinada vers�o, independente do tipo, por vers�o e total.
*/
void
totalpartsbycarversion(Matriz *modelos, Matriz *pecas)
{
    int i, j, k; // Vari�veis auxiliares
	int* v; // Ponteiro para armazenar as partes
    int* total; // Ponteiro para armazenar os totais de pe�as
    
	v = (int*)malloc(pecas->linhas*sizeof(int));
	total = (int*)malloc(pecas->linhas*sizeof(int));
    
	if (v == NULL || total == NULL)
    {
        fprintf(stderr, "Out of memory\n");
		free(v);
		free(total);
		exit(1);
    }
	
	printf("\nd) quantas pecas sao utilizadas na construcao de uma determinada versao, independente do tipo? Por versao e total.\n\n");
    printf("total parts by car version\n---------------------------\n");
    for (i = 0; i < modelos->colunas; i++)
    {
        total[i] = 0;
        printf("car version %s\n", modelos->cabecalhodois[i]);
        for (j = 0; j < pecas->linhas; j++)
        {
            v[j] = 0;
            for (k = 0; k < pecas->colunas; k++)
            {
                v[j] += (modelos->matriz[k][i]) * (pecas->matriz[j][k]);
            }
            printf ("part %s = %d\n", pecas->cabecalhoum[j], v[j]);
        }
        total[i] = sum(v, pecas->linhas);
        printf("TOTAL = %d\n\n", total[i]);
    }
    printf("TOTAL OF PARTS = %d\n", sum(total, modelos->colunas));
}

/*
	validchar recebe como par�metro o valor inteiro de um char,
	Sua fun��o � verificar se o char � v�lido para multiplica��o de matrizes, retornando 1 se sim e 0 caso contr�rio
*/
int
validchar (int c)
{
	// Na ASCII, '1' vale 49, '9' vale 57, espa�o ' ' vale 32, '\n' vale 10, TAB '	' vale 9, '\0' vale 0
	if ((c > 48 && c < 58) || c == 32 || c == 10 || c == 9 || c == 0)
		return 1;
	
	return 0;
}

/*
    printMatriz recebe como par�metro uma struct Matriz,
    Sua fun��o � imprimir todos os dados dessa struct.
*/
void printMatriz (Matriz *m) {
    int i, j;

    for (i = 0; i < m->linhas; i++)
        printf("%s ", m->cabecalhoum[i]);
    printf("\n");
    for (i = 0; i < m->colunas; i++)
        printf ("%s ", m->cabecalhodois[i]);
    printf("\n");
    for (i = 0; i < m->linhas; i++)
    {
        for (j = 0; j < m->colunas; j++)
            printf("%d ", m->matriz[i][j]);
        printf("\n");
    }
}

