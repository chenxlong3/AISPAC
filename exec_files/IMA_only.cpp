#include <iostream>
#include "Experiments.h"
#include "CommonFuncs.h"

int main(int argc, char *argv[])
{
   InfGraph g = parseArg(argc, argv);
   run_only_IMA(g);
   return 0;
}