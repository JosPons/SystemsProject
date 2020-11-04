#include <stdio.h>
#include <stdlib.h>
#include "cmdLineParameters.h"


int main(int argc, char **argv)
{
  programParameters_t programParameters;


  initializeProgramParameters(argc, argv, &programParameters);
  printProgramParameters(programParameters);

  return 0;
}