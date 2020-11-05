#include "cmdLineParameters.h"
#include "fileIO.h"


int main(int argc, char **argv)
{
  programParameters_t programParameters;


  initializeProgramParameters(argc, argv, &programParameters);
  storeInputDatasetInMemory(programParameters.inputDirPath);
  storeQueryDatasetInClique(programParameters.queryFilePath);
  return 0;
}