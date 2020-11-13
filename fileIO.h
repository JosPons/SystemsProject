#ifndef SYSTEMSPROJECT_FILEIO_H
#define SYSTEMSPROJECT_FILEIO_H

#include <dirent.h>
#include "hashTable.h"

FILE *openInputFile(char *);
void closeInputFile(char *, FILE *);
FILE *openOutputFile(char *);
void closeOutputFile(char *, FILE *);
DIR *openDirectory(char *);
void closeDirectory(char *, DIR *);
int countNumberOfFiles(char *);
void storeInputDatasetInMemory(char *, hashTable_t *, clique_t *);
void storeQueryDatasetInClique(char *, hashTable_t, clique_t *);

#endif //SYSTEMSPROJECT_FILEIO_H
