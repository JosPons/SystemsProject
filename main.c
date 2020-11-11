#include "cmdLineParameters.h"
#include "fileIO.h"
#include <hashTable.h>


int main(int argc, char **argv)
{
  int numberOfElements;
  FILE *outputFd;
  programParameters_t programParameters;
  hashTable_t hashTable;

  /* Initialize command line and program parameters */
  initializeProgramParameters(argc, argv, &programParameters);
  numberOfElements = countNumberOfFiles(programParameters.inputDirPath);

  /* Create data structures */
  createHashTable(&hashTable, numberOfElements / 4);

  /* Store input and query datasets' data in memory */
  storeInputDatasetInMemory(programParameters.inputDirPath, &hashTable);
  storeQueryDatasetInClique(programParameters.queryFilePath, hashTable);

  /* Print diagnostic information to output file */
  outputFd = openOutputFile(programParameters.outputFilePath);
  printHashTable(hashTable, outputFd);
  closeOutputFile(programParameters.outputFilePath, outputFd);

  /* Destroy data structures */
  destroyHashTable(hashTable);
  return 0;
}
