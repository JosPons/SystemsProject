#ifndef SYSTEMSPROJECT_FILEIO_H
#define SYSTEMSPROJECT_FILEIO_H

#include <dirent.h>

FILE *openFile(char *);
void closeFile(char *, FILE *);
DIR *openDirectory(char *);
void closeDirectory(char *, DIR *);
void storeInputDatasetInMemory(char *);
void storeQueryDatasetInClique(char *);

#endif //SYSTEMSPROJECT_FILEIO_H
