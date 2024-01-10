#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H
#include "InfGraph.h"
#include "CommonFuncs.h"
#include "Algs.h"
#include "Timer.h"

InfGraph parseArg(int argn, char *argv[])
{
    string folder_name = "";
    string probability_mode = "NONE";
    double epsilon = 0;
    string model = "";
    uint32_t k_seed=0, k_edges = 0, beta = 1;
    uint32_t rand_seed = 2023;
    double delta = 0.0;
    CascadeModel casc_model;
    bool fast_truncated = false;
    uint32_t num_cand_edges;
    string seed_mode = "IM";

    // For test
    model = "IC";
    k_seed = 50;
    k_edges = 50;

    for (int i = 0; i < argn; i++) {
        if (argv[i] == string("-dataset")) folder_name = string(argv[i + 1]) + "/";
        if (argv[i] == string("-epsilon")) epsilon = atof(argv[i + 1]);
        if (argv[i] == string("-k_seed")) k_seed = atoi(argv[i + 1]);
        if (argv[i] == string("-k_edges")) k_edges = atoi(argv[i + 1]);
        if (argv[i] == string("-num_cand_edges")) num_cand_edges = atoi(argv[i + 1]);
        if (argv[i] == string("-delta")) delta = atof(argv[i+1]);
        if (argv[i] == string("-rand_seed")) rand_seed = atoi(argv[i + 1]);
        if (argv[i] == string("-beta")) beta = atoi(argv[i + 1]);
        if (argv[i] == string("-probability")) probability_mode = string(argv[i + 1]);
        if (argv[i] == string("-seed_mode")) seed_mode = string(argv[i + 1]);

        if (argv[i] == string("-fast")) {
        string str_fast = string(argv[i+1]);
        if (str_fast == string("True")) fast_truncated = true;
        else if (str_fast == string("False")) fast_truncated = false;
        else {
            ExitMessage("Illegal input of fast: should be True or False");
        }
        }
        if (argv[i] == string("-model")) {
            if (argv[i + 1] == string("LT")) {
                model = argv[i + 1];
            } else if (argv[i + 1] == string("IC")) {
                model = argv[i + 1];
            } else
                ExitMessage("model should be IC or LT");
        }
    } 

    if (folder_name == "")
        ExitMessage("argument dataset missing");
    if (k_seed == 0)
        ExitMessage("argument k_seed missing");
    if (k_edges == 0)
        ExitMessage("argument k_edges missing");

    if (epsilon == 0)
        ExitMessage("argument epsilon missing");
    if (delta == 0.0)
        ExitMessage("argument delta missing");

    if (model == "")
        ExitMessage("argument model missing");

    string graph_file;
    if (model == "IC") {
        // graph_file = "edgelist_ic.txt";
        graph_file = "vec.graph";
        casc_model = IC;
    }
    else if (model == "LT") {
        graph_file = "edgelist_lt.txt";
        casc_model = LT;
    }

    dsfmt_gv_init_gen_rand(rand_seed);
    InfGraph g(folder_name, graph_file, probability_mode);
//    g.set_probability(probability_mode);
    g._seed_mode = seed_mode;
    g._truncated_or_not = fast_truncated;
    g.set_args(k_seed, k_edges, num_cand_edges, epsilon, delta, casc_model, beta);
    g.set_rand_seed(rand_seed);

    return g;
}

void test_generate_cand_edges_with_rand_seeds() {
    log_info("Test Generating candidate edges");
    InfGraph g("../data/DBLP/");
    log_info("Start initializing the seeds");
    g.set_seed();
    log_info("Finish initializing the seeds, please check.");
    g.generate_candidate_edges(50);
    log_info("Finish generating, please check.");
}

void test_IMA() {
    InfGraph g("../data/DBLP/");
    log_info("Start generating RR sets");
    g.set_seed();
    log_info("--- Generating the candidate edges ---");
    g.generate_candidate_edges(100);
    log_info("--- Initialize the marginal coverage of every candidate edge ---");
    g.init_seed_cov();
    log_info("Start IMA");
    g.IMA();
    print_vector(g._vec_selected_edges);
}


void evaluate_G_inf_IMA(InfGraph& g, uint32_t num_mc, string edges_mode="IMA", uint32_t log_step = 5) {
    log_info("--- Start evaluating ---");
    VecDouble inf_spread;
    uint32_t upper = g.args.k_edges;
    VecuInt32 log_points;
    for (uint32_t i = 0; i <= upper; i += log_step) {
        log_points.push_back(i);
    }
    string selected_edges_filename = g._cur_working_folder + string("selected_edges_") + edges_mode + string(".txt");
    std::vector<UVWEdge> vec_selected_edges = read_UVWEdges(selected_edges_filename);
    log_info("--- Using edges selected by " + edges_mode + " ---");
    if (vec_selected_edges.size() == 0) {
        log_info("VITAL: The edges_mode is not legal!!!");
        return;
    }
    ASSERT(g._seed_set_to_augment.size() > 0);
    ASSERT(log_points.back() <= vec_selected_edges.size());
    inf_spread.push_back(inf_evaluate(g._G, g._seed_set_to_augment, g.args.model, num_mc));
    for (int i = 0; i < log_points.size() - 1; i++) {
        log_info("--- Start adding edges --- k=" + to_string(log_points[i]));
        g.add_edges(vec_selected_edges, log_points[i], log_points[i + 1]);
        log_info("--- Start simulation ---");
        double spread = inf_evaluate(g._G, g._seed_set_to_augment, g._casc_model, num_mc);
        log_info(spread);
        inf_spread.push_back(spread);
    }

    string filename = g._cur_working_folder + string("k_inf_spread_") + edges_mode + string(".csv");
    ofstream outfile(filename);
    log_info("--- Saving Experiment Results ---");
    outfile << 'k' << ',' << "expected spread" << '\n';
    for (uint32_t i = 0; i < log_points.size(); i++)
    {
        outfile << log_points[i] << ',' << inf_spread[i] << '\n';
    }
    outfile.close();
    return;
}

void evaluate_inf_by_cov(InfGraph& g, string edges_mode="IMA", uint32_t log_step = 5) {
    log_info("--- Start evaluating ---");
    VecDouble inf_spread;
    uint32_t upper = g.args.k_edges;
    VecuInt32 log_points;
    for (uint32_t i = 0; i <= upper; i += log_step) {
        log_points.push_back(i);
    }
    string selected_edges_filename = g._cur_working_folder + string("selected_edges_") + edges_mode + string(".txt");
    std::vector<UVWEdge> vec_selected_edges = read_UVWEdges(selected_edges_filename);
    log_info("--- Using edges selected by " + edges_mode + " ---");
    if (vec_selected_edges.size() == 0) {
        log_info("VITAL: The edges_mode is not legal!!!");
        return;
    }
    ASSERT(g._seed_set_to_augment.size() > 0);
    ASSERT(log_points.back() <= vec_selected_edges.size());
    ASSERT(g._cur_RRsets_num == 0);
    g.clean_RRsets_InfGraph();
    IEM::generate_RRsets_for_estimate(g);
    inf_spread.push_back(g.comp_inf_by_cov(g._seed_set_to_augment));
    for (int i = 0; i < log_points.size() - 1; i++) {
        log_info("--- Start adding edges --- k=" + to_string(log_points[i]));
        g.add_edges(vec_selected_edges, log_points[i], log_points[i + 1]);
        log_info("--- Start simulation ---");
        g.clean_RRsets_InfGraph();
        // g.build_RRsets(ceil(g.n * log(g.n)), true);
        IEM::generate_RRsets_for_estimate(g);
        inf_spread.push_back(g.comp_inf_by_cov(g._seed_set_to_augment));
    }

    string filename = g._cur_working_folder + string("k_inf_spread_") + edges_mode + string(".csv");
    ofstream outfile(filename);
    log_info("--- Saving Experiment Results ---");
    outfile << 'k' << ',' << "expected spread" << '\n';
    for (uint32_t i = 0; i < log_points.size(); i++)
    {
        outfile << log_points[i] << ',' << inf_spread[i] << '\n';
    }
    outfile.close();
    return;
}

void run_only_IMA(InfGraph& g) {
    g.create_param_dir();

    log_info("--- Setting seed set ---");
    if (check_file_exist(g._seed_filename)) {
        g.set_seed("IM", true);
    } else {
        if (g._seed_mode == "IM")
        {
            IMM::influence_maximize(g, g.args);
            g.set_seed("IM", false);
            g.clean_RRsets_InfGraph();
        }
        else {
            g.set_seed(g._seed_mode, false);
        }
    }
    if (check_file_exist(g._cand_edges_filename)) {
        log_info("--- Reading the candidate edges ---");
        g.read_cand_edges();
    } else {
        log_info("--- Generating the candidate edges ---");
        if (g.args.num_cand_edges == 0) {
         g.generate_candidate_edges();
        } else
         g.generate_candidate_edges(g.args.num_cand_edges);
    }
    string timer_name = "IMA_" + g.folder;
    Timer timer = Timer(timer_name.c_str());
    string param_folder = g._cur_working_folder;
    string log_filename = param_folder + "logs.txt";
    ofstream log_file(log_filename);
    log_info("--- Start generating RR sets ---");
    // IEM::generate_RRsets(g);
    IE::stopping_rules(g);
    timer.log_operation_time("RR set generation", log_file);

    g.IMA();
    timer.log_till_now("IMA", log_file);

    g.select_edges_by_single_inf();
    g.select_edges_by_updated_inf();
    g.select_edges_by_AIS_U();
    g.select_random_edges();
    g.select_edges_by_outdegree();
    timer.log_operation_time("Select by outdeg", log_file);
    g.select_edges_by_prob();
    timer.log_operation_time("Select by prob", log_file);
}

void run_only_RWGreedy(InfGraph& g) {
    g.create_param_dir();

    log_info("--- Setting seed set ---");
    if (check_file_exist(g._seed_filename)) {
        g.set_seed("IM", true);
    } else {
        if (g._seed_mode == "IM")
        {
            IMM::influence_maximize(g, g.args);
            g.set_seed("IM", false);
            g.clean_RRsets_InfGraph();
        }
        else {
            g.set_seed(g._seed_mode, false);
        }
    }
    if (check_file_exist(g._cand_edges_filename)) {
        log_info("--- Reading the candidate edges ---");
        g.read_cand_edges();
    } else {
        log_info("--- Generating the candidate edges ---");
        if (g.args.num_cand_edges == 0) {
         g.generate_candidate_edges();
        } else
         g.generate_candidate_edges(g.args.num_cand_edges);
    }
    string timer_name = "RWGreedy_" + g.folder;
    Timer timer = Timer(timer_name.c_str());
    string param_folder = g._cur_working_folder;
    string log_filename = param_folder + "logs_RWGreedy.txt";
    ofstream log_file(log_filename);
    // log_info("--- Start generating RR sets ---");
    timer.log_operation_time("RR set generation", log_file);
    g.select_edges_by_RWGreedy();
    timer.log_till_now("RWGreedy", log_file);

}

void run_IMA_for_outdeg_seeds(InfGraph& g) {
    g.create_param_dir();
    
    string timer_name = "IMA_" + g.folder;
    Timer timer = Timer(timer_name.c_str());

    log_info("--- Setting seed set ---");
    if (check_file_exist(g._seed_filename)) {
        g.set_seed("IM", true);
    } else {
        IMM::influence_maximize(g, g.args);
        g.set_seed("IM", false);
        g.clean_RRsets_InfGraph();
    }

    if (check_file_exist(g._cand_edges_filename)) {
        log_info("--- Reading the candidate edges ---");
        g.read_cand_edges();
    } else {
        log_info("--- Generating the candidate edges ---");
        if (g.args.num_cand_edges == 0) {
         g.generate_candidate_edges();
        } else
         g.generate_candidate_edges(g.args.num_cand_edges);
        timer.log_operation_time("Edge candidate set generation");
    }
    string param_folder = g._cur_working_folder;
    string log_filename = param_folder + "logs.txt";
    ofstream log_file(log_filename);
    log_info("--- Start generating RR sets ---");
    IEM::generate_RRsets(g);

    timer.log_operation_time("RR set generation", log_file);    

    g.IMA();
    timer.log_total_time(log_file);

    g.select_random_edges();
    g.select_edges_by_outdegree();
    timer.log_operation_time("Select random / select by outdeg");

    timer.log_total_time();
}

void experiment_efficacy(InfGraph& g) {
    g.create_param_dir();
    
    string timer_name = "IMA_" + g.folder;
    Timer timer = Timer(timer_name.c_str());

    log_info("--- Setting seed set ---");
    if (check_file_exist(g._seed_filename)) {
        g.set_seed("IM", true);
    } else {
        IMM::influence_maximize(g, g.args);
        g.set_seed("IM", false);
        g.clean_RRsets_InfGraph();
    }

    if (check_file_exist(g._cand_edges_filename)) {
        log_info("--- Reading the candidate edges ---");
        g.read_cand_edges();
    } else {
        g.generate_candidate_edges(g.args.num_cand_edges);
        log_info("--- Generating the candidate edges ---");
        timer.log_operation_time("Edge candidate set generation");
    }
    string param_folder = g._cur_working_folder;
    string log_filename = param_folder + "logs.txt";
    ofstream log_file(log_filename);
    log_info("--- Start generating RR sets ---");
    IEM::generate_RRsets(g);

    timer.log_operation_time("RR set generation");    

    g.IMA();
    timer.log_total_time(log_file);

    g.select_random_edges();
    g.select_edges_by_outdegree();
    timer.log_operation_time("Select random / select by outdeg");
    log_file.close();
    return;
}


void select_outdegree(InfGraph& g) {
    g.create_param_dir();
    g.init_from_files();
    g.select_edges_by_outdegree();
    return;
}

void run_MC_Greedy(InfGraph& g, uint32_t num_mc) {
    g.create_param_dir();
    
    string timer_name = "MCGreedy_" + g.folder;
    Timer timer = Timer(timer_name.c_str());

    string log_filename = g._cur_working_folder + "logs_MCGreedy_" + to_string(num_mc) + ".txt";
    ofstream log_file(log_filename);

    log_info("--- Setting seed set ---");
    if (check_file_exist(g._seed_filename)) {
        g.set_seed("IM", true);
    } else {
        if (g._seed_mode == "IM")
        {
            IMM::influence_maximize(g, g.args);
            g.set_seed("IM", false);
            g.clean_RRsets_InfGraph();
        }
        else {
            g.set_seed(g._seed_mode, false);
        }
    }
    if (check_file_exist(g._cand_edges_filename)) {
        log_info("--- Reading the candidate edges ---");
        g.read_cand_edges();
    } else {
        g.generate_candidate_edges(g.args.num_cand_edges);
        log_info("--- Generating the candidate edges ---");
        timer.log_operation_time("Edge candidate set generation");
    }
    g.select_edges_by_MC_Greedy(num_mc);
    timer.log_total_time(log_file);
    return;
}

void experiment_evaluate_from_file(InfGraph& g, string edges_mode="IMA", uint32_t log_step=5, string eval_mode="MC") {
    g.create_param_dir();
    g.init_from_files(false);
    if (eval_mode == "MC")
        evaluate_G_inf_IMA(g, 10000, edges_mode, log_step);
    else
        evaluate_inf_by_cov(g, edges_mode, log_step);    
    return;
}

#endif