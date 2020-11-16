#include "cmdLineParameters.h"
#include "fileIO.h"


int main(int argc, char **argv)
{
  int numberOfElements;
  FILE *outputFd;
  programParameters_t programParameters;
  hashTable_t hashTable;
  clique_t clique;

  /* Initialize command line and program parameters */
  initializeProgramParameters(argc, argv, &programParameters);
  numberOfElements = countNumberOfFiles(programParameters.inputDirPath);

  /* Create data structures */
  createClique(&clique, numberOfElements);
  createHashTable(&hashTable, numberOfElements / 4);

  /* Store input and query datasets' data in memory */
  storeInputDatasetInMemory(programParameters.inputDirPath, &hashTable, &clique);
  storeQueryDatasetInClique(programParameters.queryFilePath, hashTable, &clique);

  /* Print diagnostic information to output file */
  outputFd = openOutputFile(programParameters.outputFilePath);
//  printHashTable(hashTable, outputFd);
//  printClique(clique, outputFd);
  printPositiveCliquePairs(&clique, outputFd);
//  printNegativeCliquePairs(&clique, outputFd);
  closeOutputFile(programParameters.outputFilePath, outputFd);

  /* Destroy data structures */
  destroyClique(clique);
  destroyHashTable(hashTable);
  return 0;
}