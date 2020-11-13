#include <stdlib.h>
#include <string.h>
#include "clique.h"


void createClique(clique_t *clique, int cliqueSize)
{

  clique->cliqueArray = calloc(cliqueSize, sizeof(cliqueElement_t));
  if (clique->cliqueArray == NULL)
  {
    perror("Error, couldn't allocate memory for \"cliqueArray\"");
    exit(1);
  }
  clique->cliqueSize = cliqueSize;
  for (int i = 0; i < cliqueSize; i++)
  {
    clique->cliqueArray[i].posIndexNext = -1;
    clique->cliqueArray[i].posIndexPrev = -1;
    clique->cliqueArray[i].negIndexNext = -1;
    clique->cliqueArray[i].negIndexPrev = -1;
  }
}

void destroyClique(clique_t clique)
{
  free(clique.cliqueArray);
}

void insertClique(clique_t *clique, int itemId, char *itemSiteName, int cliqueArrayIndex)
{
  strcpy(clique->cliqueArray[cliqueArrayIndex].specData.itemSiteName, itemSiteName);
  clique->cliqueArray[cliqueArrayIndex].specData.itemId = itemId;
  clique->cliqueArray[cliqueArrayIndex].posIndexNext = cliqueArrayIndex;
  clique->cliqueArray[cliqueArrayIndex].posIndexPrev = cliqueArrayIndex;
  clique->cliqueArray[cliqueArrayIndex].negIndexNext = cliqueArrayIndex;
  clique->cliqueArray[cliqueArrayIndex].negIndexPrev = cliqueArrayIndex;
}

void insertConnectionToClique(clique_t *clique, int leftSpecCliqueIndex, int rightSpecCliqueIndex, int matchFlag)
{
  /**
   * Right->previous->next = Left->next
   * Left->next->previous  = Right->previous
   * Left->next            = Right
   * Right->previous       = Left
   */

  if (matchFlag == 1)
  {
    int rightPrevious = clique->cliqueArray[rightSpecCliqueIndex].posIndexPrev;
    int leftNext = clique->cliqueArray[leftSpecCliqueIndex].posIndexNext;

    /* 1st step */
    clique->cliqueArray[rightPrevious].posIndexNext = clique->cliqueArray[leftSpecCliqueIndex].posIndexNext;
    /* 2nd step */
    clique->cliqueArray[leftNext].posIndexPrev = clique->cliqueArray[rightSpecCliqueIndex].posIndexPrev;
    /* 3rd step */
    clique->cliqueArray[leftSpecCliqueIndex].posIndexNext = rightSpecCliqueIndex;
    /* 4th step */
    clique->cliqueArray[rightSpecCliqueIndex].posIndexPrev = leftSpecCliqueIndex;
  }
  else if (matchFlag == 0)
  {
    int rightPrevious = clique->cliqueArray[rightSpecCliqueIndex].negIndexPrev;
    int leftNext = clique->cliqueArray[leftSpecCliqueIndex].negIndexNext;

    /* 1st step */
    clique->cliqueArray[rightPrevious].negIndexNext = clique->cliqueArray[leftSpecCliqueIndex].negIndexNext;
    /* 2nd step */
    clique->cliqueArray[leftNext].negIndexPrev = clique->cliqueArray[rightSpecCliqueIndex].negIndexPrev;
    /* 3rd step */
    clique->cliqueArray[leftSpecCliqueIndex].negIndexNext = rightSpecCliqueIndex;
    /* 4th step */
    clique->cliqueArray[rightSpecCliqueIndex].negIndexPrev = leftSpecCliqueIndex;
  }
  else
  {
    printf("Error, incorrect match flag code: %d\n", matchFlag);
    exit(1);
  }
}

void printPositiveCliquePairs(clique_t *clique, FILE *outputFd)
{
  int outerCliqueIndex;
  int tempOuterCliqueIndex;
  int innerCliqueIndex;
  cliqueElement_t outerCliqueElement;
  cliqueElement_t tempOuterCliqueElement;
  cliqueElement_t innerCliqueElement;

  fprintf(outputFd, "------------------------------------------------------\n");
  fprintf(outputFd, "Neighbouring clique pairs:\n");
  fprintf(outputFd, "------------------------------------------------------\n");
  for (int i = 0; i < clique->cliqueSize; i++)
  {
    if (clique->cliqueArray[i].posPrintFlag == 1)
      continue;

    outerCliqueIndex = i;
    tempOuterCliqueIndex = i;
    innerCliqueIndex = clique->cliqueArray[i].posIndexNext;

    outerCliqueElement = clique->cliqueArray[outerCliqueIndex];
    tempOuterCliqueElement = clique->cliqueArray[tempOuterCliqueIndex];
    innerCliqueElement = clique->cliqueArray[innerCliqueIndex];

    while (strcmp(outerCliqueElement.specData.itemSiteName, innerCliqueElement.specData.itemSiteName) != 0 ||
           outerCliqueElement.specData.itemId != innerCliqueElement.specData.itemId)
    {
      while (outerCliqueIndex != innerCliqueIndex)
      {
        fprintf(outputFd, "\t%s_%d , %s_%d\n",
                tempOuterCliqueElement.specData.itemSiteName,
                tempOuterCliqueElement.specData.itemId,
                innerCliqueElement.specData.itemSiteName,
                innerCliqueElement.specData.itemId);
        clique->cliqueArray[innerCliqueIndex].posPrintFlag = 1;
        innerCliqueIndex = innerCliqueElement.posIndexNext;
        innerCliqueElement = clique->cliqueArray[innerCliqueIndex];
      }
      tempOuterCliqueIndex = tempOuterCliqueElement.posIndexNext;
      tempOuterCliqueElement = clique->cliqueArray[tempOuterCliqueIndex];
      innerCliqueIndex = tempOuterCliqueElement.posIndexNext;
      innerCliqueElement = clique->cliqueArray[innerCliqueIndex];
    }
    clique->cliqueArray[outerCliqueIndex].posPrintFlag = 1;
  }
}

void printNegativeCliquePairs(clique_t *clique, FILE *outputFd)
{
  int outerCliqueIndex;
  int tempOuterCliqueIndex;
  int innerCliqueIndex;
  cliqueElement_t outerCliqueElement;
  cliqueElement_t tempOuterCliqueElement;
  cliqueElement_t innerCliqueElement;

  fprintf(outputFd, "------------------------------------------------------\n");
  fprintf(outputFd, "Non-Neighbouring clique pairs:\n");
  fprintf(outputFd, "------------------------------------------------------\n");
  for (int i = 0; i < clique->cliqueSize; i++)
  {
    if (clique->cliqueArray[i].negPrintFlag == 1)
      continue;

    outerCliqueIndex = i;
    tempOuterCliqueIndex = i;
    innerCliqueIndex = clique->cliqueArray[i].negIndexNext;

    outerCliqueElement = clique->cliqueArray[outerCliqueIndex];
    tempOuterCliqueElement = clique->cliqueArray[tempOuterCliqueIndex];
    innerCliqueElement = clique->cliqueArray[innerCliqueIndex];

    while (strcmp(outerCliqueElement.specData.itemSiteName, innerCliqueElement.specData.itemSiteName) != 0 ||
           outerCliqueElement.specData.itemId != innerCliqueElement.specData.itemId)
    {
      while (outerCliqueIndex != innerCliqueIndex)
      {
        fprintf(outputFd, "\t%s_%d , %s_%d\n",
                tempOuterCliqueElement.specData.itemSiteName,
                tempOuterCliqueElement.specData.itemId,
                innerCliqueElement.specData.itemSiteName,
                innerCliqueElement.specData.itemId);
        clique->cliqueArray[innerCliqueIndex].negPrintFlag = 1;
        innerCliqueIndex = innerCliqueElement.negIndexNext;
        innerCliqueElement = clique->cliqueArray[innerCliqueIndex];
      }
      tempOuterCliqueIndex = tempOuterCliqueElement.negIndexNext;
      tempOuterCliqueElement = clique->cliqueArray[tempOuterCliqueIndex];
      innerCliqueIndex = tempOuterCliqueElement.negIndexNext;
      innerCliqueElement = clique->cliqueArray[innerCliqueIndex];
    }
    clique->cliqueArray[outerCliqueIndex].negPrintFlag = 1;
  }
}

void printClique(clique_t clique, FILE *outputFd)
{
  fprintf(outputFd, "------------------------------------------------------\n");
  fprintf(outputFd, "Clique array size: %u\n", clique.cliqueSize);
  fprintf(outputFd, "------------------------------------------------------\n");
  for (int i = 0; i < clique.cliqueSize; i++)
  {
    fprintf(outputFd, "------------------------------------------------------\n");
    fprintf(outputFd, "Clique[%d]\n", i);
    fprintf(outputFd, "\tItem Site Name : %s\n", clique.cliqueArray[i].specData.itemSiteName);
    fprintf(outputFd, "\tItem Id        : %d\n", clique.cliqueArray[i].specData.itemId);
    fprintf(outputFd, "\tPositive Index Next     : %d\n", clique.cliqueArray[i].posIndexNext);
    fprintf(outputFd, "\tPositive Index Previous : %d\n", clique.cliqueArray[i].posIndexPrev);
    fprintf(outputFd, "\tNegative Index Next     : %d\n", clique.cliqueArray[i].negIndexNext);
    fprintf(outputFd, "\tNegative Index Previous : %d\n", clique.cliqueArray[i].negIndexPrev);
    fprintf(outputFd, "\tPositive Print Flag : %d\n", clique.cliqueArray[i].posPrintFlag);
    fprintf(outputFd, "\tNegative Print Flag : %d\n", clique.cliqueArray[i].negPrintFlag);
    fprintf(outputFd, "------------------------------------------------------\n");
  }
}