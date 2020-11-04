#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "cmdLineParameters.h"
#include "fileIO.h"


int main(int argc, char **argv)
{
  programParameters_t programParameters;


  initializeProgramParameters(argc, argv, &programParameters);
  storeInputDatasetInMemory(programParameters.inputDirPath);

  return 0;
}