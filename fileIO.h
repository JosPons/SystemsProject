#ifndef SYSTEMSPROJECT_FILEIO_H
#define SYSTEMSPROJECT_FILEIO_H


DIR *openDirectory(char *);
void closeDirectory(char *, DIR *);
void storeInputDatasetInMemory(char *);

#endif //SYSTEMSPROJECT_FILEIO_H
