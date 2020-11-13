#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashTable.h"
#include "murmur3.h"

#define BUCKET_SIZE 4
#define SEED 83 // Arbitrary seed value used by 'Murmur' hash function.


//***************************************************************************************************//
void createHashTable(hashTable_t *hashTable, int hashTableSize)
{
  hashTable->hashTableArray = calloc(hashTableSize, sizeof(struct bucket_t));
  if (hashTable->hashTableArray == NULL)
  {
    perror("Error, couldn't allocate memory for \"hashTable\"");
    exit(1);
  }
  hashTable->hashTableSize = hashTableSize;
  hashTable->hashTableElements = 0;
  hashTable->cliqueFreeArrayIndex = 0;
  for (int i = 0; i < hashTableSize; i++)
  {
    hashTable->hashTableArray[i].bucketArray = calloc(BUCKET_SIZE, sizeof(struct bucketElement_t));
    if (hashTable->hashTableArray[i].bucketArray == NULL)
    {
      perror("Error, couldn't allocate memory for \"hashTable bucket\"");
      exit(1);
    }
    hashTable->hashTableArray[i].bucketSize = BUCKET_SIZE;
    hashTable->hashTableArray[i].bucketElements = 0;
    hashTable->hashTableArray[i].freeBucketIndex = 0;
  }
}

void destroyHashTable(hashTable_t hashTable)
{
  for (int i = 0; i < hashTable.hashTableSize; i++)
    free(hashTable.hashTableArray[i].bucketArray);
  free(hashTable.hashTableArray);
}

void insertHashTable(hashTable_t *hashTable, char *key, char *itemSiteName, int itemId, clique_t *clique)
{
  uint64_t hashKey128[4];
  uint32_t hashKey;
  int freeBucketIndex;

  MurmurHash3_x64_128(key, (int) strlen(key), SEED, hashKey128);
  assert(hashKey128[0] >= 0);
  hashKey = hashKey128[0] % hashTable->hashTableSize;
  freeBucketIndex = hashTable->hashTableArray[hashKey].freeBucketIndex;
  if (hashTable->hashTableArray[hashKey].bucketElements == hashTable->hashTableArray[hashKey].bucketSize)
    resizeBucket(&hashTable->hashTableArray[hashKey]);
  /* Update the fields of the element in the hash table */
  strcpy(hashTable->hashTableArray[hashKey].bucketArray[freeBucketIndex].itemSiteName, itemSiteName);
  hashTable->hashTableArray[hashKey].bucketArray[freeBucketIndex].itemId = itemId;
  hashTable->hashTableArray[hashKey].bucketArray[freeBucketIndex].cliqueArrayIndex = hashTable->cliqueFreeArrayIndex;
  /* Also insert the element in the clique and initialize it's fields */
  insertClique(clique, itemId, itemSiteName, hashTable->cliqueFreeArrayIndex);
  /* Update the fields of the bucket, where the element belongs to */
  hashTable->hashTableArray[hashKey].bucketElements++;
  hashTable->hashTableArray[hashKey].freeBucketIndex++;
  /* Update the number of elements of the hash table and the next available
   * clique index, where we 'll add the next element */
  hashTable->hashTableElements++;
  hashTable->cliqueFreeArrayIndex++;
}

int searchHashTable(hashTable_t hashTable, char *key, char *itemSiteName, int itemId)
{
  uint64_t hashKey128[4];
  uint32_t hashKey;
  bucket_t bucket;
  bucketElement_t bucketElement;

  MurmurHash3_x64_128(key, (int) strlen(key), SEED, hashKey128);
  assert(hashKey128[0] >= 0);
  hashKey = hashKey128[0] % hashTable.hashTableSize;
  bucket = hashTable.hashTableArray[hashKey];
  for (int i = 0; i < bucket.bucketElements; i++)
  {
    bucketElement = bucket.bucketArray[i];
    if ((strcmp(bucketElement.itemSiteName, itemSiteName)) == 0 && (bucketElement.itemId == itemId))
      return bucketElement.cliqueArrayIndex;
  }
  return -1;
}
//***************************************************************************************************//
//***************************************************************************************************//
void resizeBucket(bucket_t *bucket)
{
  int oldSize = bucket->bucketSize;
  bucket->bucketSize = bucket->bucketSize * 2;
  bucket->bucketArray = realloc(bucket->bucketArray, sizeof(bucketElement_t) * bucket->bucketSize);
  if (bucket->bucketArray == NULL)
  {
    perror("Error, couldn't re-allocate memory for \"bucketArray\"");
    exit(1);
  }
  memset(bucket->bucketArray + oldSize, 0, sizeof(bucketElement_t) * oldSize);
}

void printHashTable(hashTable_t hashTable, FILE *outputFd)
{
  fprintf(outputFd, "------------------------------------------------------\n");
  fprintf(outputFd, "Hash table size: %d\n", hashTable.hashTableSize);
  fprintf(outputFd, "Hash table elements: %d\n", hashTable.hashTableElements);
  fprintf(outputFd, "Hash table free clique index: %d\n", hashTable.cliqueFreeArrayIndex);
  fprintf(outputFd, "------------------------------------------------------\n");
  for (int i = 0; i < hashTable.hashTableSize; i++)
  {
    fprintf(outputFd, "------------------------------------------------------\n");
    fprintf(outputFd, "Hash[%u] bucketSize: %d\n", i, hashTable.hashTableArray[i].bucketSize);
    fprintf(outputFd, "Hash[%u] bucketElements: %d\n", i, hashTable.hashTableArray[i].bucketElements);
    fprintf(outputFd, "Hash[%u] freeBucketIndex: %d\n", i, hashTable.hashTableArray[i].freeBucketIndex);
    fprintf(outputFd, "------------------------------------------------------\n");
    for (int j = 0; j < hashTable.hashTableArray[i].bucketSize; j++)
    {
      fprintf(outputFd, "\tBucket[%u] Site name: %s\n", j, hashTable.hashTableArray[i].bucketArray[j].itemSiteName);
      fprintf(outputFd, "\tBucket[%u] Item   Id: %d\n", j, hashTable.hashTableArray[i].bucketArray[j].itemId);
      fprintf(outputFd, "\tBucket[%u] Clique Id: %d\n", j, hashTable.hashTableArray[i].bucketArray[j].cliqueArrayIndex);
    }
  }
}
//***************************************************************************************************//