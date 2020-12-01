#ifndef SYSTEMSPROJECT_CLIQUE_H
#define SYSTEMSPROJECT_CLIQUE_H

#include <stdio.h>

typedef union jsonvalue
{
  char *stringValue;
  struct _jsonobject *jsonObject;
} jsonvalue;

typedef struct keyValuePair_t
{
  char *key;
  jsonvalue *value;
} keyValuePair_t;

typedef struct elementData_t
{
  char itemSiteName[FILENAME_MAX];
  int itemId;
  keyValuePair_t *elementSpecs;
  int SpecsSize;
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
  int elementBelongsToPosClique;
  int elementBelongsToNegClique;
} cliqueElement_t;

typedef struct clique_t
{
  cliqueElement_t *cliqueArray;
  int cliqueSize;
} clique_t;

void createClique(clique_t *, int);

void destroyClique(clique_t);

void insertClique(clique_t *, int, char *, int, char *);

void insertConnectionToClique(clique_t *, int, int, int);

void printPositiveCliquePairs(clique_t *, FILE *);

int strNextChar(char *, char);

void resizeElementData_t(elementData_t *);

void parseJsonGen(elementData_t *,char *);

void parseJsonRecur(elementData_t *, char *,int *);

void printNegativeCliquePairs(clique_t *, FILE *);

void printClique(clique_t, FILE *);

#endif //SYSTEMSPROJECT_CLIQUE_H
