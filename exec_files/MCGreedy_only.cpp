#include <iostream>
#include "Experiments.h"
#include "CommonFuncs.h"

int main(int argc, char *argv[])
{
   InfGraph g = parseArg(argc, argv);
   uint32_t num_mc = -1;
   for (int i = 0; i < argc; i++) {
      if (argv[i] == string("-num_mc")) num_mc = atoi(argv[i + 1]);
   }

   if (num_mc == -1) {
      ExitMessage("Please select num_mc");
   }
   run_MC_Greedy(g, num_mc);
   return 0;
}