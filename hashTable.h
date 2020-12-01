#ifndef SYSTEMSPROJECT_HASHTABLE_H
#define SYSTEMSPROJECT_HASHTABLE_H

#include <stdio.h>
#include "clique.h"

typedef struct bucketElement_t
{
  char itemSiteName[FILENAME_MAX];
  int itemId;
  int cliqueArrayIndex;
} bucketElement_t;

typedef struct bucket_t
{
  bucketElement_t *bucketArray;
  int bucketSize;
  int bucketElements;
  int freeBucketIndex;
} bucket_t;

typedef struct hashTable_t
{
  bucket_t *hashTableArray;
  int hashTableSize;
  int hashTableElements;
  int cliqueFreeArrayIndex;
} hashTable_t;

void createHashTable(hashTable_t *, int);

void destroyHashTable(hashTable_t);

void insertHashTable(hashTable_t *, char *, char *, int, clique_t *, char *);

int searchHashTable(hashTable_t, char *, char *, int);

void resizeBucket(bucket_t *);

void printHashTable(hashTable_t, FILE *);

#endif //SYSTEMSPROJECT_HASHTABLE_H
