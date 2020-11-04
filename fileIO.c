#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "fileIO.h"


DIR *openDirectory(char *path)
{
  DIR *inputDirFd;

  inputDirFd = opendir(path);
  if (inputDirFd == NULL)
  {
    fprintf(stderr, "Error, couldn't open directory at \"%s\" : ", path);
    perror("");
    exit(1);
  }
  return inputDirFd;
}

void closeDirectory(char *path, DIR *fd)
{
  if (closedir(fd) != 0)
  {
    fprintf(stderr, "Error, couldn't close directory at \"%s\" : ", path);
    perror("");
    exit(1);
  }
}

void storeInputDatasetInMemory(char *path)
{
  char rootDirectoryName[FILENAME_MAX];
  char subDirectoryName[FILENAME_MAX];
  char fileName[FILENAME_MAX];
  int numberOfDirectories = 0;
  int numberOfFiles = 0;
  struct dirent *dirInfo;
  DIR *DirectoryFd;
  DIR *directoryFileFd;

  DirectoryFd = openDirectory(path);
  strcpy(rootDirectoryName, path);
  printf("Root-Directory name: %s\n", rootDirectoryName);
  /* Iterate through each sub-directory */
  while ((dirInfo = readdir(DirectoryFd)) != NULL)
  {
    if (strcmp(dirInfo->d_name, ".") == 0 || strcmp(dirInfo->d_name, "..") == 0)
      continue;
    strcpy(subDirectoryName, path);
    strcat(strcat(subDirectoryName, "\\"), dirInfo->d_name);
    printf("Sub-Directory name: %s\n", subDirectoryName);

    /* Iterate through each sub-directory file*/
    directoryFileFd = openDirectory(subDirectoryName);
    while ((dirInfo = readdir(directoryFileFd)) != NULL)
    {
      if (strcmp(dirInfo->d_name, ".") == 0 || strcmp(dirInfo->d_name, "..") == 0)
        continue;
      strcpy(fileName, subDirectoryName);
      strcat(strcat(fileName, "\\"), dirInfo->d_name);
      printf("Sub-Directory file name: %s\n", fileName);
      numberOfFiles++;
    }
    closeDirectory(subDirectoryName, directoryFileFd);
    numberOfDirectories++;
  }
  printf("Number of directories: %d\n", numberOfDirectories);
  printf("Number of files: %d\n", numberOfFiles);
  closeDirectory(path, DirectoryFd);
}