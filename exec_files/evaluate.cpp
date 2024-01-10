#include "Experiments.h"
#include "CommonFuncs.h"

int main(int argc, char *argv[])
{
   InfGraph g = parseArg(argc, argv);
   string edges_mode = "";
   string eval_mode = "MC";
   uint32_t log_step = -1;
   for (int i = 0; i < argc; i++) {
      if (argv[i] == string("-edges_mode")) edges_mode = string(argv[i + 1]);
      if (argv[i] == string("-log_step")) log_step = atoi(argv[i + 1]);
      if (argv[i] == string("-eval_mode")) eval_mode = string(argv[i + 1]);
   }
   if (edges_mode == "") {
      log_info("Please select edges mode");
      return 0;
   }
   if (log_step == -1) {
      ExitMessage("Please select log_step");
   }
   
   experiment_evaluate_from_file(g, edges_mode, log_step, eval_mode);
   return 0;
}