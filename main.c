#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

Hugo Sousa Nasciutti - 11621BSI260
Caliton Marcos Goncalves Junior - 11711BSI261
Joao Barbosa de Souza Neto - 1711BSI212

*/

struct fileContent
{
  int rows;
  int cols;
  int **matrix;
  char **rowsNames;
  char **colsNames;
};
typedef struct fileContent FILECONTENT;

FILECONTENT * getContent(char *fileName);
int argsOk(int argc, char *argv[]);
int getFileSize(char *fileName);
char *sanitizeString(char *string);
int getRowArgs(char *string);

int main(int argc, char *argv[]) {
  FILECONTENT *pecas;
  getContent(argv[1]);

  return 0;
}

FILECONTENT *
getContent(char *fileName) {
  int size = getFileSize(fileName);
  int rowArgs = 0;
  char *buffer = (char *) malloc(size * sizeof(char));
  FILE *fp = fopen(fileName, "r");
  if(!fp) {
    fprintf(stderr, "%s: file not found.\n", fileName);
    return 0;
  }
  fread(buffer, sizeof(char), size, fp);
  buffer[size - 1] = '\0';
  buffer = sanitizeString(buffer);
  rowArgs = getRowArgs(buffer);
  printf("%d\n", rowArgs);
  fclose(fp);

  return NULL;
}
int getRowArgs(char *string) {
  int i = 0, args = 0;
  while(string[i] != '\n') {
    if(string[i] == ' ') {
      args++;
    }
    i++;
  }
  return args+1;
}

char *
sanitizeString(char *string) {
  // Remove more than 1 space of a string.
  // Example: A    B => A B

  int i = 0, j = 0;
  int spacesCount = 0;
  char c;
  while(c != '\0') {
    c = string[i];
    if(c == ' ') {
      spacesCount++;
    }
    if(c != ' ' && spacesCount > 1) {
      j = i;
      spacesCount -= 1;
      while(c != '\0') {
        c = string[j];
        string[j-spacesCount] = c;
        j++;
      }
      c = string[i];
    }
    if(c != ' ') {
      spacesCount = 0;
    }
    i++;
  }

  return string;
}

int
getFileSize(char *fileName) {
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

int
argsOk(int argc, char *argv[])
{
  if(argc < 3) {
    fprintf(stderr, "%s: missing arguments\n", argv[0]);
    return 0;
  }

  return 1;
}
