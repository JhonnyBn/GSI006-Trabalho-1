# GSI006-Trabalho-1
Trabalho da matéria GSI006 - Estruturas de Dados 1

Trabalho: Indústria Automobilística
===================================
(baseado em https://www.youtube.com/watch?v=yPvH8M67DHQ)

Uma indústria automobilística produz N tipos de veículo em M versões, utilizando P tipos de peças. A quantidade de veículos e de peças envolvidas na produção dos mesmos estão contidos em dois arquivos.

Um arquivo contem a quantidade de veículos produzidos em suas M versões. Esse arquivo possui N + 2 linhas:
- a primeira linha contém N strings, cada um representando o nome de um tipo de veículo;

- a segunda linha contém M strings, cada um representando o nome de uma versão de veículo;

- as demais N linhas contêm, em cada uma, M números, representando, respectivamente, a quantidade de veículos produzidos em cada versão.

Um EXEMPLO de conteúdo do arquivo descrito imediatamente acima é apresentado a seguir.

X Y
ST LX LXS
2 3 4
3 2 5

A primeira linha apresenta o nome dos tipos de veículos fabricados: X e Y. A segunda apresenta o nome das versões desses tipos de veículo: ST, LX e LXS. Portanto, são produzidos N = 2 tipos de veículos (X e Y), cada um em M = 3 versões (ST, LX e LXS). As demais linhas apresentam, respectivamente, as quantidades de veículos produzidos nessas versões. A linha 2 3 4, por exemplo, informa que, com relação ao veículo do tipo X, foram produzidos: 2 unidades da versção ST, 3 da versão LX e 4 da LXS.Por sua vez, a linha 3 2 5 informa que, com relação ao veículo tipo Y, foram produzidos: 3 unidade da versão ST, 2 da versão LX e 5 da versão LXS.

O outro arquivo contém a quantidade de peças utilizadas na montagem dos N tipos de veículos. Esse arquivo contém P + 2 linhas:
- a primeira linha contém P strings, cada um representando o nome de um tipo de peça;

- a segunda linha contém N strings, cada um representando o nome de um tipo de veículo;

- as demais P linhas contêm, em cada uma, N números, representando respectivamente, a quantidade de peças utilizadas em cada tipo de veículo.

Um EXEMPLO de conteúdo do arquivo descrito imediatamente acima é apresentado a seguir.

A B C
X Y
4 3
3 5
6 2

A primeira linha apresenta o nome dos tipos de peças utilizados pela indústria: A, B e C. A segunda linha apresenta o nome dos tipos de veículos fabricados: X e Y. Portanto, são utilizados P = 3 tipos de peças (A, B e C) na construção dos N = 2 tipos de veículos (X e Y). As demais linhas apresentam, respectivamente, a quantidade de peças utilizadas em cada um dos tipos de veículo. A linha 4 3, por exemplo, informa que, com relação à peça A, foram utilizadas: 4 unidades no veículo X e 3 no Y. A linha 3 5 informa que, com relação à peça B, foram utilizadas: 3 unidades no veículo tipo X e 5 no Y. Finalmente, a linha 6 2 informa que, com relação à peça tipo C, foram utilizadas: 6 unidades no veículo tipo X e 2 no Y.


Problema
--------
O diretor de planejamento da indústria autmobilística deseja resposta para as seguintes preguntas:

a) quantos veículos de um determinado tipo foram construídos? Por versão e total.

b) quantos veículos de uma determinada versão foram construídos? Por tipo e total.

c) quantas peças são utilizadas na produção de um determinado tipo de veículo em suas diferentes versões? Por versão e total.

d) quantas peças são utilizadas na construção de uma determinada versão, independente do tipo? Por versão e total.


O que fazer
-----------
Um programa que apresente resposta para as perguntas apresentadas na Seção Problema.


Restrições
----------
1. A linguagem de programação é C.

2. O uso do padrão de codificação da disciplina é obrigatório. O topo do arquivo deve conter, além dos comentários esperados, o nome de cada um dos membros do grupo.

3. O trabalho deve ser realizado em grupo de 3 alunos matriculados em GSI006 - Estrutura de Dados 1. A escolha dos membros de cada grupo é exclusiva dos alunos.

4. O nome dos arquivos que contém os dados sobre a produção dos automóveis devem ser informados em tempo de lançamento do programa.

   O conteúdo dos arquivos não é conhecido previamente.

   O nome do arquivos podem ser passados para o programa em QUALQUER ordem.

5. Os dados de cada arquivo devem ser carregados em uma matriz específica. As matrizes devem estar declaradas na função main. Portanto, as matrizes são variáveis locais da função main.

   Uma matriz é uma struct que possui 4 membros:

	- cabeçalho da matriz (que guarda os dados das duas primeiras linhas da matriz);
	- número de linhas da matriz;
	- numero de colunas da matriz;
	- endereço da matriz, que guarda números inteiros > 0;
	- código de carga, que indica se ocorreu algum erro na carga da matriz. Esse código é 0, se a carga ocorreu sem erro, ou > 0 se ocorreu algum erro durante a carga.

6. O uso de modularização do código é mandatório. Portanto, é obrigatório a especificação de várias funções para o programa. A função main deve ter a chamada para essas funções.

   Ao ler função main, o leitor deve ter facilidade em entender o "roteiro" do programa.

7. Espera-se encontrar no programa as seguintes funções.

   7.1 	Uma função que verifique se os argumentos passados para o programa estão presentes. Retorna 1, se o conteúdo de argc e argv[] estão consistentes; ou retorna 0, caso contrário.Os parâmetros dessa função são argc e argv[].

   7.2 	Uma função que carregue o conteúdo de um arquivo em uma matriz. Essa função retorna o endereço da struct que contém os dados sobre a matriz; ou retorna NULL, se malloc falhar. Portanto, essa função deve alocar espaço para a matriz, carregar os dados na matriz e retornar o endereço da struct que contém a matriz (vide item 5).

	O único parêmtro da função é o nome no arquivo que contém os dados que se quer carregar na matriz.

   7.3	Uma função para apresentar as respostas de cada uma das perguntas, especificadas na Seção Problema. Portanto, deve haver 4 funções, todas  do tipo void; cada uma apresenta resposta para uma pergunta. Os parêmetros dessas funções são as matrizes

   7.4 	Outras funções que se fizerem necessárias para modularizar as funções especificadas em 7.1 - 7.3.

8. Os protótipos das funções especificadas no item 7 devem estar em um arquivo específico do tipo .h. O arquivo .c deve dar "include" nesse arquivo.

9. As mensagens de erro devem ser enviadas para stderr.

10. As respostas às perguntas especificadas na Seção Problema devem ser apresentadas uma após a outra, sem qualquer intervenção do usuário. Portanto, o programa NÃO é interativo.

    As respostas devem estar inteligíveis de modo que seja possível identificar a qual pergunta uma resposta corresponde.

11. O programa deve ser compilado pelo GCC. O código executável deve ser executável em um sistema operacional baseado no Linux.

12. Um arquivo tipo txt com nome "readme" deve incluir informaçãoes sobre o programa de modo geral. Essas informações devem incluir quais os erros que podem ocorrer durante a execução do programa e quais as correspondentes medidas para lidar com tais erros.


O que entregar
--------------
Um arquivo do zip (ou gzip) contendo os arquivos .c, .h e readme.txt correspondentes a este trabalho.


Como entregar
-------------
Este trabalho de ver ser entregue por email. A mensagem de envio do trabalho deve ter seus campos preenchidos como abaixo.

Remetente:	um membro do grupo com cópia para TODOS os demais.

Destinatério: 	email do professor (ufu.br)

Assunto: 	[GSI006] 2017/2 trabalho: auto

Corpo da mensagem: nome e número de matrícula de cada um dos membros
		   do grupo

Anexo: 	o arquivo mencionado na Seção "O que entregar"

	O nome do arquivo .zip deve ter o seguinte leiaute:

	2017-2-gsi006-auto-mat.zip

	onde mat deve ser substituído pelo número de matrícula do
	membro do grupo que está enviando a mensagem.
