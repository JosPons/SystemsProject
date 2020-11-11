#ifndef SYSTEMSPROJECT_CMDLINEPARAMETERS_H
#define SYSTEMSPROJECT_CMDLINEPARAMETERS_H

#include <stdio.h>


typedef struct programParameters_t
{
  char inputDirPath[FILENAME_MAX];
  char queryFilePath[FILENAME_MAX];
  char outputFilePath[FILENAME_MAX];
} programParameters_t;

void initializeProgramParameters();
void printProgramParameters();

#endif //SYSTEMSPROJECT_CMDLINEPARAMETERS_H
