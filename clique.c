#include <stdlib.h>
#include <string.h>
#include "clique.h"
#include "fileIO.h"

#define SPECS_SIZE 10
#define isWhitespace(x)                         x == '\r' || x == '\n' || x == '\t' || x == ' '

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
    clique->cliqueArray[i].specData.elementSpecs = calloc(SPECS_SIZE, sizeof(keyValuePair_t));
    if (clique->cliqueArray[i].specData.elementSpecs == NULL)
    {
      perror("Error, couldn't allocate memory for \"elementSpecs\"");
      exit(1);
    }
  }
}

void destroyClique(clique_t clique)
{
  for (int i = 0; i < clique.cliqueSize; i++)
    free(clique.cliqueArray[i].specData.elementSpecs);
  free(clique.cliqueArray);
}

void resizeElementData_t(elementData_t *elementData)
{
  int oldSize = elementData->SpecsSize;
  elementData->SpecsSize = elementData->SpecsSize * 2;
  elementData->elementSpecs = realloc(elementData->elementSpecs, sizeof(keyValuePair_t) * elementData->SpecsSize);
  if (elementData->elementSpecs == NULL)
  {
    perror("Error, couldn't re-allocate memory for \"bucketArray\"");
    exit(1);
  }
  memset(elementData->elementSpecs + oldSize, 0, sizeof(keyValuePair_t) * oldSize);
}


void insertClique(clique_t *clique, int itemId, char *itemSiteName, int cliqueArrayIndex, char *jsonFile)
{
  strcpy(clique->cliqueArray[cliqueArrayIndex].specData.itemSiteName, itemSiteName);
  clique->cliqueArray[cliqueArrayIndex].specData.itemId = itemId;
  parseJsonGen(&clique->cliqueArray[cliqueArrayIndex].specData, writeJsonString(jsonFile));
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
    if (clique->cliqueArray[leftSpecCliqueIndex].elementBelongsToPosClique == 1)
    {
      int innerCliqueIndex = clique->cliqueArray[leftSpecCliqueIndex].posIndexNext;
      while (innerCliqueIndex != leftSpecCliqueIndex)
      {
        /* If both elements appear in the same clique, dont connect them again and return */
        if (innerCliqueIndex == rightSpecCliqueIndex)
          return;
        innerCliqueIndex = clique->cliqueArray[innerCliqueIndex].posIndexNext;
      }
    }

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

    clique->cliqueArray[leftSpecCliqueIndex].elementBelongsToPosClique = 1;
    clique->cliqueArray[rightSpecCliqueIndex].elementBelongsToPosClique = 1;
  } else if (matchFlag == 0)
  {
    if (clique->cliqueArray[leftSpecCliqueIndex].elementBelongsToNegClique == 1)
    {
      int innerCliqueIndex = clique->cliqueArray[leftSpecCliqueIndex].negIndexNext;
      while (innerCliqueIndex != leftSpecCliqueIndex)
      {
        /* If both elements appear in the same clique, dont connect them again and return */
        if (innerCliqueIndex == rightSpecCliqueIndex)
          return;
        innerCliqueIndex = clique->cliqueArray[innerCliqueIndex].negIndexNext;
      }
    }

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

    clique->cliqueArray[leftSpecCliqueIndex].elementBelongsToNegClique = 1;
    clique->cliqueArray[rightSpecCliqueIndex].elementBelongsToNegClique = 1;
  } else
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

int strNextChar(char *str, char ch)
{
  int pos = 0;

  if (str == NULL)
    return -1;

  while (*str != ch && *str != '\0')
  {
    str++;
    pos++;
  }
  if (*str == '\0')
    return -1;
  else
    return pos;
}

void parseJsonGen(elementData_t *elementData, char *jsonStr)
{
  int offset = 0;
  parseJsonRecur(elementData, jsonStr, &offset);
}

void parseJsonRecur(elementData_t *elementData, char *jsonStr, int *offset)
{

  int offsetRecur = 0;
  while (*jsonStr != '\0')
  {
    while (isWhitespace(*jsonStr))  //remove white spaces
    {
      jsonStr++;
      offsetRecur++;
    };
    if (*jsonStr == '{')
    {
      jsonStr++;
      offsetRecur++;
    } else if (*jsonStr == '"')
    {

      int i = strNextChar(++jsonStr, '"');
      if (i > 1)
      {
        
        keyValuePair_t tempPtr = elementData->elementSpecs[elementData->SpecsSize - 1];

        tempPtr.key = (char *) malloc(i + 1 * sizeof(char));
        memcpy(tempPtr.key, jsonStr, i * sizeof(char));
        tempPtr.key[i] = '\0';

        jsonStr += i + 1;
        offsetRecur += i + 2;

        i = strNextChar(jsonStr, ':');
        if (i == -1)
          break;
        jsonStr += i + 1;
        offsetRecur += i + 1;

        while (isWhitespace(*jsonStr))
        {
          jsonStr++;
          offsetRecur++;
        };

        if (*jsonStr == '{')
        {
          int offsetRecurBeforeParsingChildOb = offsetRecur;
          int sizeOfChildObj;

          tempPtr.value = (jsonvalue *) malloc(sizeof(jsonvalue));
          parseJsonRecur(tempPtr.value->jsonObject, jsonStr, &offsetRecur);       //reminder:check recursive for seg
          if (tempPtr.value->jsonObject == NULL)
          {
            break;
          }
          sizeOfChildObj = offsetRecur - offsetRecurBeforeParsingChildOb;     //move pointer after the json childarray
          jsonStr += sizeOfChildObj;
        } else if (*jsonStr == '"')
        {
          i = strNextChar(++jsonStr, '"');
          if (i == -1)
          {
            break;
          }
          tempPtr.value = (jsonvalue *) malloc(sizeof(jsonvalue));
          tempPtr.value->stringValue = (char *) malloc(i + 1 * sizeof(char));
          memcpy(tempPtr.value->stringValue, jsonStr, i * sizeof(char));
          tempPtr.value->stringValue[i] = '\0';
          jsonStr += i + 1;
          offsetRecur += i + 2;
        }
        elementData->elementSpecs[elementData->SpecsSize - 1] = tempPtr;
      }
    } else if (*jsonStr == ',')
    {
      elementData->elementSpecs++;
      if (elementData->SpecsSize > SPECS_SIZE)
        resizeElementData_t(elementData);      //if specsize exceeds starting size ,expand

      jsonStr++;
      offsetRecur++;
    } else if (*jsonStr == '}')
    {
      (*offset) += offsetRecur + 1;

    }
  }

}