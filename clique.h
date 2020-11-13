#ifndef SYSTEMSPROJECT_CLIQUE_H
#define SYSTEMSPROJECT_CLIQUE_H

#include <stdio.h>

typedef struct elementData_t
{
  char itemSiteName[FILENAME_MAX];
  int itemId;
} elementData_t;

typedef struct cliqueElement_t
{
  elementData_t specData;
  int posIndexNext;
  int posIndexPrev;
  int negIndexNext;
  int negIndexPrev;
  int posPrintFlag;
  int negPrintFlag;
} cliqueElement_t;

typedef struct clique_t
{
  cliqueElement_t *cliqueArray;
  int cliqueSize;
} clique_t;

void createClique(clique_t *, int);
void destroyClique(clique_t);
void insertClique(clique_t *, int, char *, int);
void insertConnectionToClique(clique_t *, int, int, int);
void printPositiveCliquePairs(clique_t *, FILE *);
void printNegativeCliquePairs(clique_t *, FILE *);
void printClique(clique_t, FILE *);

#endif //SYSTEMSPROJECT_CLIQUE_H
