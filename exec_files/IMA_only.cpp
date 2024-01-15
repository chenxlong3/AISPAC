#include <iostream>
#include "Experiments.h"
#include "CommonFuncs.h"

int main(int argc, char *argv[])
{
   // get mode
   string func = "";
   for (int i = 0; i < argc; i++) {
      if (argv[i] == string("-func")) func = string(argv[i + 1]);
   }
   if (func == "format") {
       format_graph(argc, argv);
       return 1;
   }
   
   InfGraph g = parseArg(argc, argv);
   run_only_IMA(g);
   // run_only_AISPAC(g);
   return 1;
}