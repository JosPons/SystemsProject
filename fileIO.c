#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileIO.h"


FILE *openInputFile(char *path)
{
  FILE *fd;

  fd = fopen(path, "r");
  if (fd == NULL)
  {
    fprintf(stderr, "Error, couldn't open file at \"%s\" : ", path);
    perror("");
    exit(1);
  }
  return fd;
}

void closeInputFile(char *path, FILE *fd)
{
  if (fclose(fd) != 0)
  {
    fprintf(stderr, "Error, couldn't close file at \"%s\" : ", path);
    perror("");
    exit(1);
  }
}

FILE *openOutputFile(char *path)
{
  FILE *fd;

  fd = fopen(path, "w");
  if (fd == NULL)
  {
    fprintf(stderr, "Error, couldn't open file at \"%s\" : ", path);
    perror("");
    exit(1);
  }
  return fd;
}

void closeOutputFile(char *path, FILE *fd)
{
  if (fclose(fd) != 0)
  {
    fprintf(stderr, "Error, couldn't close file at \"%s\" : ", path);
    perror("");
    exit(1);
  }
}

DIR *openDirectory(char *path)
{
  DIR *dirFd;

  dirFd = opendir(path);
  if (dirFd == NULL)
  {
    fprintf(stderr, "Error, couldn't open directory at \"%s\" : ", path);
    perror("");
    exit(1);
  }
  return dirFd;
}

void closeDirectory(char *path, DIR *dirFd)
{
  if (closedir(dirFd) != 0)
  {
    fprintf(stderr, "Error, couldn't close directory at \"%s\" : ", path);
    perror("");
    exit(1);
  }
}

int countNumberOfFiles(char *path)
{
  char rootDirectoryName[FILENAME_MAX];
  char subDirectoryName[FILENAME_MAX];
  char fileName[FILENAME_MAX];
  int numberOfFiles = 0;
  struct dirent *dirInfo;
  DIR *DirectoryFd;
  DIR *directoryFileFd;

  DirectoryFd = openDirectory(path);
  strcpy(rootDirectoryName, path);
  /* Iterate through each sub-directory */
  while ((dirInfo = readdir(DirectoryFd)) != NULL)
  {
    if (strcmp(dirInfo->d_name, ".") == 0 || strcmp(dirInfo->d_name, "..") == 0)
      continue;
    strcpy(subDirectoryName, path);
//    strcat(strcat(subDirectoryName, "\\"), dirInfo->d_name); // For Windows paths
    strcat(strcat(subDirectoryName, "/"), dirInfo->d_name); // For linux paths
    /* Iterate through each sub-directory file*/
    directoryFileFd = openDirectory(subDirectoryName);
    while ((dirInfo = readdir(directoryFileFd)) != NULL)
    {
      if (strcmp(dirInfo->d_name, ".") == 0 || strcmp(dirInfo->d_name, "..") == 0)
        continue;
      strcpy(fileName, subDirectoryName);
//      strcat(strcat(fileName, "\\"), dirInfo->d_name); // For Windows paths
      strcat(strcat(fileName, "/"), dirInfo->d_name); // For linux paths
      numberOfFiles++;
    }
    closeDirectory(subDirectoryName, directoryFileFd);
  }
  closeDirectory(path, DirectoryFd);
  return numberOfFiles;
}

void storeInputDatasetInMemory(char *path, hashTable_t *hashTable, clique_t *clique)
{
  char rootDirectoryName[FILENAME_MAX];
  char subDirectoryName[FILENAME_MAX];
  char fileName[FILENAME_MAX];
  char hashTableKey[FILENAME_MAX];
  char siteName[FILENAME_MAX];
  int itemId;
  int numberOfFiles = 0;
  char *endptr; // For strtol() function use
  struct dirent *dirInfo;
  DIR *directoryFd;
  DIR *directoryFileFd;

  strcpy(rootDirectoryName, path);
  /* Iterate through each sub-directory */
  directoryFd = openDirectory(path);
  while ((dirInfo = readdir(directoryFd)) != NULL)
  {
    if (strcmp(dirInfo->d_name, ".") == 0 || strcmp(dirInfo->d_name, "..") == 0)
      continue;
    strcpy(subDirectoryName, path);
//    strcat(strcat(subDirectoryName, "\\"), dirInfo->d_name); // For Windows paths
    strcat(strcat(subDirectoryName, "/"), dirInfo->d_name); // For linux paths
    strcpy(siteName, dirInfo->d_name);

    /* Iterate through each sub-directory file*/
    directoryFileFd = openDirectory(subDirectoryName);
    while ((dirInfo = readdir(directoryFileFd)) != NULL)
    {
      if (strcmp(dirInfo->d_name, ".") == 0 || strcmp(dirInfo->d_name, "..") == 0)
        continue;
      strcpy(fileName, subDirectoryName);
//      strcat(strcat(fileName, "\\"), dirInfo->d_name); // For Windows paths
      strcat(strcat(fileName, "/"), dirInfo->d_name); // For linux paths

      /* Convert the itemId from string to integer */
      itemId = (int) strtol(dirInfo->d_name, &endptr, 0);
      /* Create the key, which we will pass on the hashtable for indexing */
      strcpy(hashTableKey, siteName);
      strcat(hashTableKey, strtok(dirInfo->d_name, "."));
      /* Insert the item with hashKey, siteName and itemId in the hash table */
      insertHashTable(hashTable, hashTableKey, siteName, itemId, clique);
      numberOfFiles++;
    }
    closeDirectory(subDirectoryName, directoryFileFd);
  }
  closeDirectory(path, directoryFd);
}

void storeQueryDatasetInClique(char *path, hashTable_t hashTable, clique_t *clique)
{
  char lineBuffer[FILENAME_MAX];
  char leftSpecSite[FILENAME_MAX];
  char rightSpecSite[FILENAME_MAX];
  char leftSpecId[FILENAME_MAX];
  char rightSpecId[FILENAME_MAX];
  char matchFlag[2];
  char hashTableKey[FILENAME_MAX];
  char *endptr; // For strtol() function use
  int leftSpecIndex;
  int rightSpecIndex;
  FILE *fileFd;

  fileFd = openInputFile(path);
  /* Read the first line and get rid of it */
  fgets(lineBuffer, 4096, fileFd);
  while (fgets(lineBuffer, 4096, fileFd) != NULL)
  {
    /* Tokenize and store the values read to the corresponding variables */
    sscanf(lineBuffer, "%90[^/]// %[^,],%90[^/]// %[^,],%[^\n]\n",
           leftSpecSite, leftSpecId, rightSpecSite, rightSpecId, matchFlag);
    /* Create the hashKey of the left spec */
    strcpy(hashTableKey, leftSpecSite);
    strcat(hashTableKey, leftSpecId);
    leftSpecIndex = searchHashTable(hashTable, hashTableKey, leftSpecSite, (int) strtol(leftSpecId, &endptr, 0));
    /* Create the hashKey of the right spec */
    strcpy(hashTableKey, rightSpecSite);
    strcat(hashTableKey, rightSpecId);
    rightSpecIndex = searchHashTable(hashTable, hashTableKey, rightSpecSite, (int) strtol(rightSpecId, &endptr, 0));
    /* Insert pair to clique */
    insertConnectionToClique(clique, leftSpecIndex, rightSpecIndex, (int) strtol(matchFlag, &endptr, 0));
  }
  closeInputFile(path, fileFd);
}