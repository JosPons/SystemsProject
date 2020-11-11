#include <stdlib.h>
#include <string.h>
#include "cmdLineParameters.h"


void initializeProgramParameters(int argc, char **argv, programParameters_t *programParameters)
{

  /**
   * $./executable_Name
   * –d <input file>
   * –q <query file>
   */

  if (argc != 7)
  {
    printf("Expected number of arguments is 4\n");
    exit(1);
  }

  for (int i = 1; i < argc; i += 2)
  {
    if (strcmp("-d", argv[i]) == 0)
      strcpy(programParameters->inputDirPath, argv[i + 1]);
    else if (strcmp("-q", argv[i]) == 0)
      strcpy(programParameters->queryFilePath, argv[i + 1]);
    else if (strcmp("-o", argv[i]) == 0)
      strcpy(programParameters->outputFilePath, argv[i + 1]);
    else
    {
      fprintf(stderr, "Wrong type of argument prefix.\nExpected list of \\"
                      "values: \' -d -q -o \'\n");
      exit(1);
    }
  }
}

void printProgramParameters(programParameters_t programParameters)
{
  printf("Input file path: %s\n", programParameters.inputDirPath);
  printf("Query file path: %s\n", programParameters.queryFilePath);
  printf("Output file path: %s\n", programParameters.outputFilePath);
}