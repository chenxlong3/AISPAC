#ifndef INF_GRAPH_H
#define INF_GRAPH_H
#include "GraphBase.h"
#include "Timer.h"



// All arguments passed to func
class Argument{
public:
    uint32_t rand_seed = 0;
    string folder_name = "";
    string graph_file = "";
    string probability_mode = "";
    
    CascadeModel model = IC;
    string str_model = "";
    
    string seed_mode = "";
    string method="";

    uint32_t k_seed=0, k_edges = 0, beta = 1;
    double epsilon = 0;
    double delta=0;
    
    bool fast_truncated = true;
    uint32_t num_cand_edges = 0;
    size_t num_samples = 0;     // num_mc or num of RR sets
    
    Argument() {

    };
    Argument(uint32_t _k_seed, double eps) {
        this->k_seed = k_seed;
        this->epsilon = eps;
    };
    Argument(uint32_t _k_seed, uint32_t k_links, double eps) {
        this->k_seed = _k_seed;
        this->k_edges = k_links;
        this->epsilon = eps;
    };
    Argument(uint32_t _k_seed, uint32_t k_links, uint32_t _num_cand_edges, double eps, double _delta, CascadeModel _model, uint32_t _beta) {
        this->k_seed = _k_seed;
        this->k_edges = k_links;
        this->num_cand_edges = _num_cand_edges;
        this->epsilon = eps;
        this->delta = _delta;
        this->model = _model;
        this->beta = _beta;
    };
    void check_arguments_eligible() {
        if (this->folder_name == "")
            ExitMessage("argument dataset missing");
        if (this->k_seed == 0)
            ExitMessage("argument k_seed missing");
        if (this->k_edges == 0)
            ExitMessage("argument k_edges missing");
        if (this->epsilon == 0)
            ExitMessage("argument epsilon missing");
        if (this->delta == 0)
            ExitMessage("argument delta missing");

        if (this->rand_seed == 0)
            log_info("argument rand_seed missing, 0 as default");
        if (this->str_model == "")
            log_info("argument model missing, IC as default");
        if (this->method == "") {
            log_info("argument method missing, AISPAC as default");
            this->method = "AISPAC";
        }
        if (this->seed_mode == "") {
            log_info("argument seed_mode missing, IM as default");
            this->seed_mode = "IM";
        }
        if (this->probability_mode == "") {
            log_info("argument probability_mode missing, NONE as default");
            this->probability_mode == "NONE";
        }
        if (this->graph_file == "") {
            log_info("argument graph_file missing, vec.graph as default");
            this->graph_file = "vec.graph";
        }

        return;
    }
};

class InfGraph: public GraphBase {
    public:
        
        VecVecInt32 _RRsets;      // _RRsets[i] is the vector storing nodes of the i-th RRset
        VecVecLargeNum _node_to_RRsets;  // _node_to_RRsets[i] is the vector storing the indices of RR sets containing node i
        CascadeModel _casc_model = IC;
        NodeList _seed_set_IM;
        NodeList _seed_set_to_augment;
        // VecLargeNum _node_sampled_times; // For Debug
        std::vector<uint32_t> _RRset_traversed_edges;
        VecBool _vec_bool_vis;
        VecBool _vec_bool_seed;
        size_t _cur_RRsets_num=0;
        LL _num_traversed_edges=0, _tmp_num_traversed_edges=0;
        VecSetInt _SET_RRsets;
        VecLargeNum _marginal_cov;
        VecBool _vec_bool_RRset_cov_by_S;           // Bool, indicating whether a RR set is covered by S
        VecBool _vec_bool_cand_edges_selected;
        
        string _seed_mode = "RAND";

        // For Influence Estimation
        // VecVecDouble _vec_phi;
        // VecVecDouble _vec_eta;

        std::vector<UVWEdge> _vec_cand_edges;       // vector of all candidate edges
        std::vector<UVWEdge> _vec_selected_edges;   // vector of selected edges
        std::vector<UVWEdge> _vec_selected_edges_RAND;   // vector of selected edges by random
        std::vector<UVWEdge> _vec_selected_edges_OUTDEG;   // vector of selected edges by degree
        std::vector<UVWEdge> _vec_selected_edges_MCGreedy;   // vector of selected edges by degree
        std::vector<UVWEdge> _vec_selected_edges_PROB;   // vector of selected edges by degree
        std::vector<std::priority_queue<PairSizetDouble, std::vector<PairSizetDouble>, CompareBySecondDoubleForSizet>> _vec_node_prob_heap;
        
        string _cur_working_folder = "";
        string _cand_edges_filename = "";
        string _seed_filename = "";
        Argument args = Argument(50, 5, 0.3);
        bool _truncated_or_not = false;

        InfGraph(): GraphBase() {
            return;
        }
        InfGraph(string folder_name, string f_name="edgelist_ic.txt", bool format_g=true): GraphBase(folder_name, f_name, true) {
            this->_node_to_RRsets.resize(this->n);
            this->_vec_bool_vis.resize(this->n, false);
            this->_vec_bool_seed.resize(this->n, false);
            return;
        }

        InfGraph(string folder_name, string f_name, string prob_mode): GraphBase(folder_name, f_name, prob_mode) {
            this->_node_to_RRsets.resize(this->n);
            this->_vec_bool_vis.resize(this->n, false);
            this->_vec_bool_seed.resize(this->n, false);
            // this->_node_sampled_times.resize(this->n, 0);
            return;
        }

        void clean_RRsets_InfGraph() {
            VecVecInt32().swap(this->_RRsets); VecVecLargeNum().swap(this->_node_to_RRsets);
            this->_cur_RRsets_num=0;
            this->_num_traversed_edges=0;
            this->_node_to_RRsets.resize(this->n);
            return;
        }

        void set_casc_model(CascadeModel model) {
            this->_casc_model = model;
            return;
        }

        void set_args(Argument arguments) {
            this->args = arguments;
            
            this->_seed_mode = this->args.seed_mode;
            this->_cand_edges_filename = this->folder + "candidate_edges_" + to_string(this->args.num_cand_edges) + "_" + this->_seed_mode + "_" + to_string(this->args.k_seed) + this->_prob_mode + ".txt";
            this->_seed_filename = this->folder + "seeds_" + this->_seed_mode + "_" + to_string(this->args.k_seed) + this->_prob_mode + ".txt";
            
            log_info("--- Arguments ---");
            log_info("k_seed", this->args.k_seed);
            log_info("k_edges", this->args.k_edges);
            log_info("num_cand_edges", this->args.num_cand_edges);
            log_info("epsilon", this->args.epsilon);
            log_info("delta", this->args.delta);
            log_info("seed_mode", this->_seed_mode);
            log_info("random seed", this->args.rand_seed);
            log_info("beta", this->args.beta);
            log_info("num_samples", this->args.num_samples);
            return;
        }

        void set_args(uint32_t _k_seed, uint32_t k_links, uint32_t _num_cand_edges, double eps, double _delta, CascadeModel _model, uint32_t _beta) {
            this->args.k_seed = _k_seed;
            this->args.k_edges = k_links;
            this->args.num_cand_edges = _num_cand_edges;
            this->args.epsilon = eps;
            this->args.delta = _delta;
            this->args.model = _model;
            this->args.beta = _beta;
            this->_cand_edges_filename = this->folder + "candidate_edges_" + to_string(this->args.num_cand_edges) + "_" + this->_seed_mode + "_" + to_string(this->args.k_seed) + this->_prob_mode + ".txt";
            this->_seed_filename = this->folder + "seeds_" + this->_seed_mode + "_" + to_string(this->args.k_seed) + this->_prob_mode + ".txt";
            log_info("--- Arguments ---");
            log_info("k_seed", this->args.k_seed);
            log_info("k_edges", this->args.k_edges);
            log_info("num_cand_edges", this->args.num_cand_edges);
            log_info("epsilon", this->args.epsilon);
            log_info("delta", this->args.delta);
            log_info("seed_mode", this->_seed_mode);
            log_info("beta", this->args.beta);
            return;
        }

        // void set_rand_seed(uint32_t rand_seed) {
        //     this->_rand_seed = rand_seed;
        //     return;
        // }

        void set_seed(string seed_mode = "RAND", bool from_file=false) {
            ASSERT(this->_cur_working_folder != "");
            if (from_file)
            {
                string seed_filename = this->_seed_filename;
                log_info("--- Initializing seed nodes from file: " + this->_seed_filename + "---");
                ifstream seed_file(seed_filename);
                uint32_t u;

                while (seed_file >> u) {
                    this->_seed_set_to_augment.push_back(u);
                    this->_vec_bool_seed[u] = true;
                }
                seed_file.close();
                print_vector(this->_seed_set_to_augment);
                return;
            }
            
            if (seed_mode == "RAND") {
                log_info("--- Select the seed set randomly ---");
                for (uint32_t i = 0; i < this->args.k_seed; i++) {
                    uint32_t rand_node = dsfmt_gv_genrand_uint32_range(this->n);
                    // log_info(rand_node);
                    this->_seed_set_to_augment.push_back(rand_node);
                    // log_info(rand_node);
                    this->_vec_bool_seed[rand_node] = true;
                }
            } else if (seed_mode == "IM") {
                if (this->_seed_set_IM.size() == 0)
                {
                    ExitMessage("Please run IMM first.");
                }
                log_info("--- Select the seed set by IMM ---");
                this->_seed_set_to_augment = NodeList(this->_seed_set_IM.begin(), this->_seed_set_IM.end());
                for (auto& node : this->_seed_set_IM) {
                    this->_vec_bool_seed[node] = true;
                }
            }
            else if (seed_mode == "OUTDEG") {
                log_info("--- Select the seed set by OUTDEG ---");
                std::priority_queue<PairIntSizet, std::vector<PairIntSizet>, CompareBySecond> heap;
                for (size_t i = 0; i < this->n; i++) {
                    heap.push(make_pair(i, this->_G[i].size()));
                }
                for (size_t i = 0; i < this->args.k_seed; i++) {
                    this->_seed_set_to_augment.push_back(heap.top().first);
                    heap.pop();
                }
            }
            print_vector(this->_seed_set_to_augment);
            save_vector(this->_seed_set_to_augment, this->_seed_filename);
            return;
        }

        void init_RRsets_cov_by_S() {
            ASSERT(this->_cur_RRsets_num > 0);
            VecBool().swap(this->_vec_bool_RRset_cov_by_S);
            this->_vec_bool_RRset_cov_by_S.resize(this->_cur_RRsets_num, false);
            for (auto seed : this->_seed_set_to_augment) {
                for (auto RRset : this->_node_to_RRsets[seed]) {
                    this->_vec_bool_RRset_cov_by_S[RRset] = true;
                }
            }
            return;
        }

        double comp_inf_cov_by_S() {
            ASSERT(this->_vec_bool_RRset_cov_by_S.size() > 0);
            return (1.0 * this->n / this->_cur_RRsets_num) * this->_vec_bool_RRset_cov_by_S.count();
        }

        void init_seed_cov() {
            this->init_RRsets_cov_by_S();
            if (this->_vec_bool_RRset_cov_by_S.size() == 0) {
                std::cout << "ERROR: Please first run init_RRsets_cov_by_S()" << std::endl;
                exit(1);
            }

            VecLargeNum().swap(this->_marginal_cov);
            std::vector<UVWEdge>().swap(this->_vec_selected_edges);

            this->_marginal_cov.resize(this->n, 0);
            // log_info("The total number of RR sets covered by S is", std::count(this->_vec_bool_RRset_cov_by_S.begin(), this->_vec_bool_RRset_cov_by_S.end(), true));
            for (size_t i = 0; i < this->_RRsets.size(); i++) {
                if (!this->_vec_bool_RRset_cov_by_S[i]) {
                    for (auto x : this->_RRsets[i]) {
                        this->_marginal_cov[x]++;
                    }
                }
            }
            return;
        }
        
        void add_edge(UVWEdge& edge) {
            uint32_t u = edge.first, v = edge.second.first;
            double w = edge.second.second;
            this->_G[u].push_back(Edge(v, w));
            return;
        }

        void pop_edge(UVWEdge& edge) {
            uint32_t u = edge.first;
            this->_G[u].pop_back();
            return;
        }

        void add_edges(std::vector<UVWEdge>& edge_tuple, uint32_t st, uint32_t end) {
            uint32_t u, v;
            double w;
            for (uint32_t i = st; i < end; i++) {
                UVWEdge& e_tuple = edge_tuple[i];
                u = e_tuple.first;
                v = e_tuple.second.first;
                w = e_tuple.second.second;
                this->m++;
                this->_G[u].push_back(Edge(v, w));
                this->_inv_G[v].push_back(Edge(u, w));
                this->_indegree[v]++;
                this->_weighted_in_degree[v] += w;
                this->_weighted_out_degree[u] += w;
            }
            return;
        }

        bool build_one_RRset(uint32_t v, const size_t RRset_idx, const bool flag_count_edges=false, bool fast_trunc_or_not=false) {
            NodeList vec_vis_nodes;         // The list of visited nodes
            size_t num_visit_node = 0, cur_idx = 0;     // Number of activated nodes; Current traversal index
            uint32_t num_trav_edges = 0;
            // For v, the target node
            this->_node_to_RRsets[v].push_back(RRset_idx);
            vec_vis_nodes.push_back(v);
            num_visit_node++;
            this->_vec_bool_vis[v] = true;
            bool end_flag = false;
            bool cov_by_S = false;
            if (this->_vec_bool_seed[v])  // If the target node is a seed node and
            {
                cov_by_S = true;
                if (fast_trunc_or_not) {
                    end_flag = true;
                }
            }

            while (cur_idx < num_visit_node && !end_flag) {         // when there exist new nodes and we have not encountered any seed node
                const auto expand = vec_vis_nodes[cur_idx++];
                num_trav_edges += this->_inv_G[expand].size();
                if (this->_casc_model == IC) {
                    for(auto& nbr: this->_inv_G[expand]) {
                        const auto nbr_id = nbr.first;
                        if (this->_vec_bool_vis[nbr_id]) {
                            // if(num_visit_node == 1) std::cout << "Skip...";
                            continue;
                        }
                        const auto rand_double = dsfmt_gv_genrand_open_close();
                        if (rand_double <= nbr.second) {
                            // The node nbr is activated
                            vec_vis_nodes.push_back(nbr_id);
                            num_visit_node++;
                            this->_vec_bool_vis[nbr_id] = true;
                            this->_node_to_RRsets[nbr_id].push_back(RRset_idx);
                            if (this->_vec_bool_seed[nbr_id]) {
                                cov_by_S = true;
                                if (fast_trunc_or_not) {
                                    end_flag = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (this->_casc_model == LT) {
                    if (this->_inv_G[expand].size() == 0) break;
                    num_trav_edges += this->_inv_G[expand].size();
                    const auto next_nbr_idx = gen_rand_node_by_weight_LT(this->_inv_G[expand]);
                    if (next_nbr_idx >= this->_inv_G[expand].size()) break; 
                    const auto nbr_id = this->_inv_G[expand][next_nbr_idx].first;
                    if (this->_vec_bool_vis[nbr_id]) break;

                    vec_vis_nodes.push_back(nbr_id);
                    num_visit_node++;
                    this->_vec_bool_vis[nbr_id] = true;
                    this->_node_to_RRsets[nbr_id].push_back(RRset_idx);
                }
            }
            
            ASSERT(num_visit_node == (size_t)vec_vis_nodes.size());
            
            for (auto i=0; i<num_visit_node; i++) this->_vec_bool_vis[vec_vis_nodes[i]] = false;
            this->_RRsets.push_back(vec_vis_nodes);
            // NodeList().swap(vec_vis_nodes);
            if (flag_count_edges) {
                this->_num_traversed_edges += num_trav_edges;
            }
            return cov_by_S;
        }

        void build_RRsets(size_t num_RRsets, bool fast_truncated=false) {
            if (num_RRsets > SIZE_MAX) {
                std::cout << "Error: The number of RR sets is too large." << std::endl;
                exit(1);   
            }
            const auto prev_size = this->_cur_RRsets_num;
            this->_cur_RRsets_num = std::max(this->_cur_RRsets_num, num_RRsets);
            std::cout << "current RR sets number: " << this->_cur_RRsets_num << std::endl;
            for (auto i=prev_size; i<num_RRsets; i++) {
                uint32_t rand_node = dsfmt_gv_genrand_uint32_range(this->n);
                // this->_node_sampled_times[rand_node]++;
                this->build_one_RRset(rand_node, i, false, fast_truncated);
            }
            ASSERT(this->_cur_RRsets_num == this->_RRsets.size());
            return;
        }
        
        double comp_inf_by_cov(const NodeList& vec_seed) {
            VecBool vec_bool_vis(this->_cur_RRsets_num);
            for (auto seed: vec_seed) {
                for(auto RRset: this->_node_to_RRsets[seed]) {
                    vec_bool_vis[RRset] = true;
                }
            }
            return 1.0 * vec_bool_vis.count() * this->n / this->_cur_RRsets_num;
        }

        double edge_selection_with_PMC(uint32_t k) {
            double delta_influence = 0.0;
        
            VecVecInt32 RRsets_delta;       // Marginal RR sets, all RR sets not covered by S
            VecVecLargeNum node_to_RRsets_delta (this->n);  // node to RR sets not covered by S
            
            // Re-index the RR sets not covered by S
            size_t cur_idx = 0;
            for(size_t i=0; i<this->_RRsets.size(); i++) {
                // if RR set i is covered by S
                if (this->_vec_bool_RRset_cov_by_S[i]) {
                    continue;
                }
                auto& RRset = this->_RRsets[i];
                
                VecuInt32 tmp_RRset;
                for(auto node: RRset) {
                    tmp_RRset.push_back(node);      // current RR set covers node
                    node_to_RRsets_delta[node].push_back(cur_idx);
                }
                RRsets_delta.push_back(tmp_RRset);
                cur_idx++;
            }
            VecDouble live_prob_per_RRset(RRsets_delta.size(), 1.0);    // store the live probability of every RR set
            log_info("Finish extrating marginal RR sets");
            // Use a heap to store the <node, prob coverage> pair
            std::priority_queue<PairIntDouble, std::vector<PairIntDouble>, CompareBySecondDouble> heap;
            VecDouble coverage(this->n, 0.0);       // coverage[v] is the expected number of RR sets covered by v
            
            for(uint32_t i=0; i<this->n; i++) {
                // store coverage
                // if the node is not covered 
                if (this->_vec_node_prob_heap[i].empty())
                    continue;
                double prob_for_i = this->_vec_node_prob_heap[i].top().second;
                double node_i_cov = prob_for_i * node_to_RRsets_delta[i].size();
                PairIntDouble tmp(make_pair(i, node_i_cov));
                heap.push(tmp);
                coverage[i] = node_i_cov;
            }
            log_info("Finish initializing marginal coverage, start selection");

            this->_vec_selected_edges.clear();
            uint32_t max_idx;
            double cov_num = 0.0;
            while (this->_vec_selected_edges.size() < k) {
                PairIntDouble top = heap.top();
                heap.pop();

                // Lazy Update
                if (top.second > coverage[top.first]) {
                    // Update coverage of top
                    top.second = coverage[top.first];
                    heap.push(top);
                    continue;
                }
                max_idx = top.first;
                cov_num += coverage[max_idx];
                double origin_cov = coverage[max_idx];
                
                // selection
                size_t top_edge_idx = this->_vec_node_prob_heap[max_idx].top().first;   // the index of the target edge
                double top_prob = this->_vec_node_prob_heap[max_idx].top().second;      // the probability of the target edge

                this->_vec_node_prob_heap[max_idx].pop();       // pop the edge for node max_idx
                this->_vec_selected_edges.push_back(this->_vec_cand_edges[top_edge_idx]);

                // After selecting one node, we need to reduce marginal gain of every node
                // e: the RR sets covered by the node max_idx
                VecLargeNum& RRsets_cov_by_max_node = node_to_RRsets_delta[max_idx];
                for (uint32_t j=0; j<RRsets_cov_by_max_node.size(); j++) {
                    size_t RRset_idx = RRsets_cov_by_max_node[j];
                    NodeList node_list = RRsets_delta[RRset_idx];
                    for (uint32_t& node: node_list){
                        if (node != max_idx && !this->_vec_node_prob_heap[node].empty()) {
                            double cur_node_prob = this->_vec_node_prob_heap[node].top().second;
                            coverage[node] -= (live_prob_per_RRset[RRset_idx] * top_prob * cur_node_prob);
                        }
                    }
                    // update RR set live prob
                    live_prob_per_RRset[RRset_idx] *= (1 - top_prob);
                }
                
                //update edges pointing to the same node
                coverage[max_idx] = 0.0;
                if (!this->_vec_node_prob_heap[max_idx].empty()) {
                    // for (uint32_t j=0; j<RRsets_cov_by_max_node.size(); j++) {
                    //     size_t RRset_idx = RRsets_cov_by_max_node[j];
                    //     coverage[max_idx] += live_prob_per_RRset[RRset_idx];
                    // }
                    
                    double new_prob = this->_vec_node_prob_heap[max_idx].top().second;
                    coverage[max_idx] = origin_cov / top_prob * new_prob * (1-top_prob);
                    // coverage[max_idx] *= new_prob;
                    heap.push(make_pair(max_idx, coverage[max_idx]));
                }
            }
            log_info("cov_num", cov_num);
            log_info("original cov", this->_vec_bool_RRset_cov_by_S.count());

            return (cov_num + this->_vec_bool_RRset_cov_by_S.count()) * this->n / this->_cur_RRsets_num;
        }

        double node_selection(uint32_t k) {
            // Use a heap to store the <node, coverage> pair
            std::priority_queue<PairIntSizet, std::vector<PairIntSizet>, CompareBySecond> heap;
            VecLargeNum coverage(this->n, 0);       // coverage[v] is the RR sets covered by v
            for(uint32_t i=0; i<this->n; i++) {
                // store coverage
                size_t node_i_cov = this->_node_to_RRsets[i].size();
                PairIntInt tmp(make_pair(i, node_i_cov));
                heap.push(tmp);
                coverage[i] = node_i_cov;
            }
            VecBool RRsets_mark(this->_RRsets.size(), false);
            VecBool node_mark(this->n, true);
            this->_seed_set_IM.clear();
            uint32_t max_idx;
            size_t cov_num = 0;
            while (this->_seed_set_IM.size() < k) {
                PairIntInt top = heap.top();
                heap.pop();
                // Lazy Update
                if (top.second > coverage[top.first]) {
                    // Update coverage of top
                    top.second = coverage[top.first];
                    heap.push(top);
                    continue;
                }
                max_idx = top.first;
                VecLargeNum e = this->_node_to_RRsets[max_idx];     // e: the RR sets covered by the node max_idx
                cov_num += coverage[max_idx];
                this->_seed_set_IM.push_back(max_idx);
                node_mark[max_idx] = false;

                // After selecting one node, we need to remove the covered RR sets from the coverage graph
                // e: the RR sets covered by the node max_idx
                for (uint32_t j=0; j<e.size(); j++) {
                    if (RRsets_mark[e[j]]) continue;        // If the RR set has been removed
                    NodeList node_list = this->_RRsets[e[j]];
                    for (uint32_t l=0; l<node_list.size(); ++l){
                        if (node_mark[node_list[l]]) coverage[node_list[l]]--;
                    }
                    RRsets_mark[e[j]] = true;
                }
            }
            return (double)cov_num / this->_cur_RRsets_num;
        }
        
        // Link Recommendation
        void generate_candidate_edges(size_t num) {
            ASSERT(this->_seed_set_to_augment.size() > 0);
            vector<UVWEdge> res;
            VecSetInt added_nodes(this->args.k_seed);
            VecBool vis_by_v(this->n, false);
            for (uint32_t i = 0; i < this->_seed_set_to_augment.size(); i++) {
                for (auto& node : this->_G[this->_seed_set_to_augment[i]]) {
                    added_nodes[i].insert(node.first);
                }
            }

            for (size_t i = 0; i < num; i++) {
                uint32_t rand_idx = dsfmt_gv_genrand_uint32_range(this->_seed_set_to_augment.size());
                uint32_t rand_node_S = this->_seed_set_to_augment[rand_idx];
                
                uint32_t rand_node = dsfmt_gv_genrand_uint32_range(this->n);
                int cnt = 0;
                while (this->_vec_bool_seed[rand_node] || added_nodes[rand_idx].count(rand_node) != 0)
                {
                    rand_idx = dsfmt_gv_genrand_uint32_range(this->_seed_set_to_augment.size());
                    rand_node_S = this->_seed_set_to_augment[rand_idx];
                    rand_node = dsfmt_gv_genrand_uint32_range(this->n);
                    cnt++;
                    if (cnt > 1000) {
                        log_info("--- FAILED to generate candidate edges in 1000 iterations ---");
                        return;
                    }
                }
                
                double w = 0.0;
                if (this->_casc_model == IC) {
                    uint32_t targ_indeg = this->_inv_G[rand_node].size(), st_outdeg = this->_G[rand_node_S].size();
                    double avg_w_in, avg_w_out;
                    if (targ_indeg == 0)
                        avg_w_in = dsfmt_gv_genrand_open_close();
                    if (st_outdeg == 0) avg_w_out = dsfmt_gv_genrand_open_close();
                    if (targ_indeg != 0 && st_outdeg != 0) {
                        avg_w_in = this->_weighted_in_degree[rand_node] / this->_inv_G[rand_node].size();
                        avg_w_out = this->_weighted_out_degree[rand_node_S] / this->_G[rand_node_S].size();
                    }

                    w = (avg_w_in + avg_w_out) / 2;
                } else
                    w = std::max(1 - this->_weighted_in_degree[rand_node], 0.0);
                
                res.push_back(make_pair(rand_node_S, Edge(rand_node, w)));
                added_nodes[rand_idx].insert(rand_node);
            }
            log_info("--- Start writing candidate edges ---");
            this->write_cand_edges(res);
            this->_vec_cand_edges = res;
            return;
        }

        void generate_candidate_edges() {
            ASSERT(this->_seed_set_to_augment.size() > 0);
            vector<UVWEdge> res;
            VecSetInt added_nodes(this->args.k_seed);

            for (uint32_t i = 0; i < this->_seed_set_to_augment.size(); i++) {
                added_nodes[i].insert(this->_seed_set_to_augment[i]);
                for (auto& node : this->_G[this->_seed_set_to_augment[i]]) {
                    added_nodes[i].insert(node.first);
                }
            }
            
            for (size_t i = 0; i < this->n; i++) {
                for (size_t j=0; j < this->args.k_seed; j++) {
                    uint32_t seed = this->_seed_set_to_augment[j];
                    double w = 0.0;
                    if (this->_casc_model == IC) {
                        uint32_t targ_indeg = this->_inv_G[i].size(), st_outdeg = this->_G[seed].size();
                        double avg_w_in, avg_w_out;
                        
                        if (targ_indeg == 0)
                            avg_w_in = dsfmt_gv_genrand_open_close();
                        if (st_outdeg == 0) avg_w_out = dsfmt_gv_genrand_open_close();
                        if (targ_indeg != 0 && st_outdeg != 0) {
                            avg_w_in = this->_weighted_in_degree[i] / targ_indeg;
                            avg_w_out = this->_weighted_out_degree[seed] / st_outdeg;
                        }
                        w = (avg_w_in + avg_w_out) / 2;
                        if (w > 1.0)
                        {
                            ExitMessage("probability over 1, please check");
                        }
                        
                    } else
                        w = std::max(1 - this->_weighted_in_degree[i], 0.0);
                    if (w > 1.0) {
                        log_info("Target's weighted indeg", this->_weighted_in_degree[i]);
                        log_info("Target's indeg", this->_inv_G[i].size());
                        log_info("Seed's weighted outdeg", this->_weighted_out_degree[seed]);
                        log_info("Seed's outdeg", this->_G[seed].size());
                        ASSERT(w <= 1.0);
                    }
                    if (!this->_vec_bool_seed[i] && added_nodes[j].count(i) == 0)
                    {
                        
                        res.push_back(make_pair(seed, Edge(i, w)));
                        added_nodes[j].insert(i);
                    }
                }
            }
            log_info("--- Writing candidate edges to " + this->_cand_edges_filename + " ---");
            this->write_cand_edges(res);
            this->_vec_cand_edges = res;
            return;
        }

        void write_cand_edges(vector<UVWEdge> edge_tuple) {
            ASSERT(this->_cur_working_folder != "");
            ofstream save_file(this->_cand_edges_filename);
            uint32_t st, end;
            double w;
            for(auto& tup : edge_tuple) {
                st = tup.first; end = tup.second.first; w = tup.second.second;
                save_file << st << '\t' << end << '\t' << w << '\n';
            }
            save_file.close();
            log_info("--- Finish saving the candidate edges. ---");
            return;
        }

        void read_cand_edges() {
            if (this->_vec_cand_edges.size() > 0) {
                log_info("--- Candidate edges already loaded ---");
                return;
            }
            // std::vector<UVWEdge>().swap(this->_vec_cand_edges);
            ASSERT(this->_vec_cand_edges.size() == 0);
            string filename = this->_cand_edges_filename;
            ifstream myfile(filename);
            uint32_t u, v;
            double w;
            while (myfile >> u >> v >> w) {
                this->_vec_cand_edges.push_back(make_pair(u, Edge(v, w)));
            }
            myfile.close();
            this->args.num_cand_edges = this->_vec_cand_edges.size();
            return;
        }


        bool aug_insert_node_to_RRset(uint32_t u, size_t RRset_idx) {
            if (this->_SET_RRsets[RRset_idx].count(u) == 0) {
                this->_RRsets[RRset_idx].push_back(u);
                this->_SET_RRsets[RRset_idx].insert(u);
                return true;
            }
            return false;
        }

        void augment_one_RRset(uint32_t u, size_t RRset_idx) {
            std::queue<uint32_t> Q;
            Q.push(u);

            while (!Q.empty()) {
                uint32_t node_id = Q.front();
                Q.pop();
                
                if (this->_casc_model == IC) {
                    for (auto& nbr : this->_inv_G[node_id]) {
                        double rand_double = dsfmt_gv_genrand_open_close();
                        bool flag_push_to_q;
                        if (rand_double <= nbr.second) {
                            flag_push_to_q = this->aug_insert_node_to_RRset(nbr.first, RRset_idx);
                            if (flag_push_to_q) Q.push(nbr.first);
                        }
                    }
                } else if (this->_casc_model == LT) {
                    if (this->_inv_G[node_id].size() == 0) break;
                    const auto next_nbr_idx = gen_rand_node_by_weight_LT(this->_inv_G[node_id]);
                    if (next_nbr_idx >= this->_inv_G[node_id].size()) break;
                    const auto nbr_id = this->_inv_G[node_id][next_nbr_idx].first;
                    bool flag_push_to_q = this->aug_insert_node_to_RRset(nbr_id, RRset_idx);
                    if (flag_push_to_q) {
                        Q.push(nbr_id);
                    }
                }
            }
            return;
        }

        void augment_RRsets(uint32_t u, uint32_t v, double w) {
            const VecLargeNum& target_RRsets = this->_node_to_RRsets[v];
            for (size_t RRset_id : target_RRsets) {
                double rand_double = dsfmt_gv_genrand_open_close();
                bool flag_add = false;
                if (this->_casc_model == IC) flag_add = (rand_double < w);
                else flag_add = (this->_RRsets[RRset_id].back() == v && rand_double < 1 - this->_weighted_in_degree[v]);
                if (rand_double < w) {
                    this->augment_one_RRset(u, RRset_id);
                } else continue;
            }
            return;
        }
        
        void augment_one_RRset_early_stop(size_t RRset_idx) {
            if(this->_vec_bool_RRset_cov_by_S[RRset_idx]) {
                return;
            }
            else {
                this->_vec_bool_RRset_cov_by_S[RRset_idx] = true;
                for (auto& node : this->_RRsets[RRset_idx]) {
                    this->_marginal_cov[node]--;
                }
            }
            return;
        }

        void augment_RRsets_early_stop(uint32_t u, uint32_t v, double w) {
            const VecLargeNum& target_RRsets = this->_node_to_RRsets[v];
            for (size_t RRset_id : target_RRsets) {
                double rand_double = dsfmt_gv_genrand_open_close();
                bool flag_add = false;
                if (this->_casc_model == IC) flag_add = (rand_double < w);
                else flag_add = (this->_RRsets[RRset_id].back() == v && rand_double < 1 - this->_weighted_in_degree[v]);
                if (flag_add) {
                    this->augment_one_RRset_early_stop(RRset_id);
                } else continue;
            }
            return;
        }

        double comp_RWGreedy_ht(UVWEdge cand_edge, size_t num_samples=10, size_t max_length=20) {
            this->add_edge(cand_edge);
            double res = 0.0;
            for (auto u : this->_seed_set_to_augment) {
                double mean_len = 0.0;
                for (size_t i = 0; i < num_samples; i++) {
                    /* code */
                    uint32_t current_node = u;
                    size_t rw_num = 0;
                    // random walk
                    while (rw_num < max_length) {
                        // Add current node to the random walk sequence
                        // You can store it in a vector or perform any other desired operation
                        // For example: random_walk_sequence.push_back(current_node);

                        // Check if the current node is not a seed
                        if (!_vec_bool_seed[current_node])
                            break;  // Random walk ends if not a seed

                        // Check if the current node has any neighbors
                        if (this->_G[current_node].empty())
                            break;  // Random walk ends if no neighbors

                        // Choose a random neighbor uniformly at random
                        size_t num_neighbors = this->_G[current_node].size();
                        size_t random_neighbor_index = dsfmt_gv_genrand_uint32_range(num_neighbors);
                        size_t next_node = this->_G[current_node][random_neighbor_index].first;

                        // Update the current node to the chosen neighbor
                        current_node = next_node;

                        // Increment the random walk length
                        rw_num++;
                    }
                    mean_len += rw_num / (double)num_samples;
                }
                res += mean_len / (double)this->args.k_seed;
            }
            
            this->pop_edge(cand_edge);
            return res;
        }

        size_t get_idx_max_marginal_gain() {
            // When the marginal coverage got changed
            // Return the index of the largest value
            double max_val = -1.0;
            size_t idx = 0;
            for (size_t i = 0; i < this->_vec_cand_edges.size(); i++) {
                uint32_t v = this->_vec_cand_edges[i].second.first;
                double w = this->_vec_cand_edges[i].second.second;
                if (!this->_vec_bool_cand_edges_selected[i]) {
                    double gain = w * this->_marginal_cov[v];
                    if (gain > max_val) {
                        max_val = gain;
                        idx = i;
                    }
                }
                // double gain = w * this->_marginal_cov[v];
                // if (gain > max_val) {
                //     max_val = gain;
                //     idx = i;
                // }
            }
            return idx;
        }
        // update the coverage
        void update_RRsets_cov_by_S(std::vector<UVWEdge>& selected_edges) {
            ASSERT(this->_RRsets.size() > 0);
            for (auto& e_tuple: selected_edges)
            {
                // update coverage after selecting
                uint32_t target_node = e_tuple.second.first;
                double w = e_tuple.second.second;
                const VecLargeNum& target_RRsets = this->_node_to_RRsets[target_node];
                
                for (size_t RRset_id : target_RRsets) {
                    double rand_double = dsfmt_gv_genrand_open_close();
                    bool flag_add = false;
                    if (this->_casc_model == IC) flag_add = (rand_double < w);
                    else flag_add = (this->_RRsets[RRset_id].back() == target_node && rand_double < 1 - this->_weighted_in_degree[target_node]);
                    if (flag_add && !this->_vec_bool_RRset_cov_by_S[RRset_id]) {
                        this->_vec_bool_RRset_cov_by_S[RRset_id] = true;
                    } else continue;
                }
            }
            return;
        }

        double estimate_by_pmc(std::vector<UVWEdge>& selected_edges) {
            ASSERT(this->_RRsets.size() > 0);
            this->init_RRsets_cov_by_S();
            double cov_num = 0.0;
            VecBool included_in_A (this->n, false);
            std::unordered_map<uint32_t, std::vector<double>> node_probs_map;
            for (auto& e_tuple: selected_edges) {
                uint32_t target_node = e_tuple.second.first;
                double w = e_tuple.second.second;
                node_probs_map[target_node].push_back(w);
                included_in_A[target_node] = true;
            }
            log_info("include in A", included_in_A.count());
            for (size_t i=0; i<this->_RRsets.size(); i++) {
                if (this->_vec_bool_RRset_cov_by_S[i]) {
                    cov_num += 1;
                    continue;
                }
                double prob_not_cov = 1.0;
                for (uint32_t node: this->_RRsets[i])
                {
                    if (included_in_A[node]) {

                        for (double p : node_probs_map[node]) {
                            prob_not_cov *= (1.0 - p);
                        }
                    }
                    
                }
                
                cov_num += (double)(1.0 - prob_not_cov);
            }
            log_info("cov by s", this->_vec_bool_RRset_cov_by_S.count());
            printf("cov-num:%.3f\n", cov_num);
            return cov_num * this->n / this->_cur_RRsets_num;
        }

        void init_from_files(bool load_cand_edges=true) {
            ASSERT(this->_cur_working_folder != "");
            log_info("--- Initializing from previous files ---");
            // string selected_edges_filename = this->_cur_working_folder + string("selected_edges_") + edges_mode + string(".txt");
            // string edges_IMA_filename = this->_cur_working_folder + string("selected_edges_IMA.txt");
            // string edges_RAND_filename = this->_cur_working_folder + string("selected_edges_RAND.txt");
            // string edges_OUTDEG_filename = this->_cur_working_folder + string("selected_edges_OUTDEG.txt");
            // string edges_PROB_filename = this->_cur_working_folder + string("selected_edges_PROB.txt");
            // string edges_MCGreedy_filename = this->_cur_working_folder + string("selected_edges_MCGreedy.txt");

            string cand_edges_file_name = this->_cand_edges_filename;
            string seed_filename = this->_seed_filename;

            uint32_t u, v;
            double w;
            if (check_file_exist(seed_filename)) {
                log_info("--- File of seed nodes: " + seed_filename + " ---");
            } else {
                log_info("--- VITAL: Please select the seeds and store it in " + seed_filename + " ---");
                return;
            }
            // If we need to load all candidate edges, please uncomment the following loop
            if (load_cand_edges) {
                if (check_file_exist(cand_edges_file_name)) {
                    log_info("--- File of candidate edges: " + cand_edges_file_name + " ---");
                    ifstream cand_edges_file(cand_edges_file_name);
                    while (cand_edges_file >> u >> v >> w) {
                        this->_vec_cand_edges.push_back(make_pair(u, Edge(v, w)));
                    }
                } else {
                    log_info("--- VITAL: Please generate the candidate edges set and store it in " + cand_edges_file_name + " ---");
                    return;
                }
            }

            ifstream seed_file(seed_filename);            

            while (seed_file >> u) {
                this->_seed_set_to_augment.push_back(u);
                this->_vec_bool_seed[u] = true;
            }
            seed_file.close();
            
            return;
        }

        void select_random_edges() {
            log_info("--- Start selecting edges randomly ---");
            ASSERT(this->_vec_cand_edges.size()>0);
            string log_filename = this->_cur_working_folder + "log_RAND.txt";
            ofstream log_file(log_filename);
            string timer_name = "RAND" + this->folder;
            Timer timer(timer_name.c_str());
            
            VecuInt32 numbers;
            for (int i = 0; i < this->_vec_cand_edges.size(); i++) {
                numbers.push_back(i); // add all numbers in the range to a vector
            }

            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                uint32_t index = dsfmt_gv_genrand_uint32_range(numbers.size());
                this->_vec_selected_edges_RAND.push_back(this->_vec_cand_edges[index]);
                numbers.erase(numbers.begin() + index);
            }
            timer.log_total_time(log_file);

            this->init_RRsets_cov_by_S();
            this->update_RRsets_cov_by_S(this->_vec_selected_edges_RAND);
            log_file << "Result by RAND: " << this->comp_inf_cov_by_S() << '\n';
            write_UVWEdges(this->_vec_selected_edges_RAND, this->_cur_working_folder+"selected_edges_RAND.txt");
            return;
        }

        void select_edges_by_outdegree() {
            log_info("--- Start selecting edges by outdegree ---");
            ASSERT(this->_vec_cand_edges.size()>0);
            string log_filename = this->_cur_working_folder + "log_OUTDEG.txt";
            ofstream log_file(log_filename);
            string timer_name = "OUTDEG" + this->folder;
            Timer timer(timer_name.c_str());

            std::priority_queue<PairIntInt, std::vector<PairIntInt>, CompareBySecond> heap;
            for (uint32_t i = 0; i < this->_vec_cand_edges.size(); i++) {
                UVWEdge& edge_tuple = this->_vec_cand_edges[i];
                heap.push(make_pair(i, this->_G[edge_tuple.second.first].size()));
            }
            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                PairIntInt top = heap.top();
                heap.pop();
                this->_vec_selected_edges_OUTDEG.push_back(this->_vec_cand_edges[top.first]);
            }
            timer.log_total_time(log_file);

            this->init_RRsets_cov_by_S();
            this->update_RRsets_cov_by_S(this->_vec_selected_edges_OUTDEG);
            log_file << "Result by OUTDEG: " << this->comp_inf_cov_by_S() << '\n';

            write_UVWEdges(this->_vec_selected_edges_OUTDEG, this->_cur_working_folder+"selected_edges_OUTDEG.txt");
            log_info("--- Finish saving the outdegree edges ---");
            return;
        }

        void select_edges_by_prob() {
            log_info("--- Start selecting edges by probability ---");
            ASSERT(this->_vec_cand_edges.size()>0);
            string log_filename = this->_cur_working_folder + "log_PROB.txt";
            ofstream log_file(log_filename);
            
            string timer_name = "PROB" + this->folder;
            Timer timer(timer_name.c_str());
            std::priority_queue<PairIntDouble, std::vector<PairIntDouble>, CompareBySecondDouble> heap;
            for (uint32_t i = 0; i < this->_vec_cand_edges.size(); i++) {
                UVWEdge& edge_tuple = this->_vec_cand_edges[i];
                heap.push(make_pair(i, edge_tuple.second.second));
            }
            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                PairIntDouble top = heap.top();
                heap.pop();
                this->_vec_selected_edges_PROB.push_back(this->_vec_cand_edges[top.first]);
            }
            timer.log_total_time(log_file);
            
            this->init_RRsets_cov_by_S();
            this->update_RRsets_cov_by_S(this->_vec_selected_edges_PROB);
            log_file << "Result by PROB: " << this->comp_inf_cov_by_S() << '\n';

            write_UVWEdges(this->_vec_selected_edges_PROB, this->_cur_working_folder+"selected_edges_PROB.txt");
            log_info("--- Finish saving the edges selected by highest probability ---");
            return;
        }

        void select_edges_by_MC_Greedy(uint32_t num_mc=100) {
            ASSERT(this->_vec_cand_edges.size() > 0);
            log_info("--- Start MC-Greedy ---");
            string log_filename = this->_cur_working_folder + "MCGreedy_" + to_string(num_mc) + "_time.txt";
            ofstream log_file(log_filename);

            string timer_name = "MCGreedy_" + to_string(num_mc) + this->folder;
            Timer timer(timer_name.c_str());
            // heap, sorting (edge idx, marginal gain) pair
            std::priority_queue<PairIntDouble, std::vector<PairIntDouble>, CompareBySecondDouble> heap;

            // Influence spread of S in the original graph
            double sigma_S = inf_evaluate(this->_G, this->_seed_set_to_augment, IC, num_mc, false);
            log_info("Origin Influence", sigma_S);
            log_info("--- Initializing marginal gains ---");
            // For rebuttal, same probability to a node
            NodeList cnt_edges_to_v(this->n, 0);
            NodeList maintain_idx(this->n, 0);
            std::vector<UVWEdge> new_vec_cand_edges;
            std::vector<NodeList> seeds_to_v(this->n);
            
            // 
            // for (size_t i = 0; i < this->_vec_cand_edges.size(); i++) {
            //     uint32_t target_node = this->_vec_cand_edges[i].second.first;
            //     seeds_to_v[target_node].push_back(this->_vec_cand_edges[i].first);

            //     if (cnt_edges_to_v[target_node] == 0 && this->_vec_cand_edges[i].second.second > 0.0) {
            //         new_vec_cand_edges.push_back(UVWEdge(seeds_to_v[target_node][0], this->_vec_cand_edges[i].second));
            //     }
            //     cnt_edges_to_v[this->_vec_cand_edges[i].second.first]++;
            // }
            // this->_vec_cand_edges = new_vec_cand_edges;
            // Initializing marginal gain vector
            for (size_t i = 0; i < this->_vec_cand_edges.size(); i++) {
                // Add the edge and evaluate the influence
                this->add_edge(this->_vec_cand_edges[i]);
                double cur_sigma = inf_evaluate(this->_G, this->_seed_set_to_augment, IC, num_mc, false);
                double Delta = cur_sigma - sigma_S;
                
                this->pop_edge(this->_vec_cand_edges[i]);
                // log_info(Delta);
                // Push to the heap
                if ((i+1) % 10000 == 0) log_info(i+1);
                heap.push(make_pair(i, Delta));
            }
            log_info("--- Finish Initializing Marginal Gains ---");

            timer.log_till_now("Initializing marginal gains", log_file);
            while (this->_vec_selected_edges_MCGreedy.size() < this->args.k_edges) {
                ASSERT(heap.size() > 0);
                double max_inf = 0.0;
                uint32_t max_idx = -1;

                PairIntDouble top = heap.top();
                heap.pop();
                max_idx = top.first;
                
                log_info("Marginal Gain", top.second);
                
                uint32_t target_node = this->_vec_cand_edges[max_idx].second.first;
                std::cout << target_node << ' ' << this->_vec_cand_edges[max_idx].second.second << std::endl;
                
                this->_vec_selected_edges_MCGreedy.push_back(this->_vec_cand_edges[max_idx]);
                
                this->add_edge(this->_vec_cand_edges[max_idx]);
                sigma_S += top.second;
                // cnt_edges_to_v[target_node]--;
                // maintain_idx[target_node]++;
                
                // if (cnt_edges_to_v[target_node] > 0) {
                //     this->_vec_cand_edges[max_idx] = UVWEdge(seeds_to_v[target_node][maintain_idx[target_node]], this->_vec_cand_edges[max_idx].second);
                //     this->add_edge(this->_vec_cand_edges[max_idx]);
                //     double cur_sigma = inf_evaluate(this->_G, this->_seed_set_to_augment, IC, num_mc, false);
                //     this->pop_edge(this->_vec_cand_edges[max_idx]);
                //     heap.push(make_pair(max_idx, cur_sigma - sigma_S));
                // }
                string rec_str = "select_" + to_string(this->_vec_selected_edges_MCGreedy.size());
                timer.log_till_now(rec_str.c_str(), log_file);
                // lazy update marginal gain
                
                uint32_t prev_top = top.first;
                // If the top keeps changing
                uint32_t tmp_cnt = 0;
                while (tmp_cnt==0 || heap.top().first != prev_top) {
                    tmp_cnt++;
                    prev_top = heap.top().first;
                    heap.pop();
                    // update marginal gain
                    this->add_edge(this->_vec_cand_edges[prev_top]);
                    double cur_sigma = inf_evaluate(this->_G, this->_seed_set_to_augment, IC, num_mc, false);
                    this->pop_edge(this->_vec_cand_edges[prev_top]);
                    // push to heap
                    heap.push(make_pair(prev_top, cur_sigma - sigma_S));
                }
                log_info("Number of simulation times", tmp_cnt);
            }
            string save_filename = "selected_edges_MCGreedy_" + to_string(num_mc) + ".txt";
            write_UVWEdges(this->_vec_selected_edges_MCGreedy, this->_cur_working_folder+save_filename);
            log_info("--- Finish MC Greedy ---");
            timer.log_total_time();
            return;
        }

        void select_edges_by_single_inf() {
            ASSERT(this->_RRsets.size() > 0);
            std::vector<UVWEdge> vec_selected_edges_single_inf;
            std::priority_queue<PairIntSizet, std::vector<PairIntSizet>, CompareBySecondSizet> heap;
            log_info("--- Start selecting by single influence ---");
            string log_filename = this->_cur_working_folder + "log_SINF.txt";
            ofstream log_file(log_filename);

            string timer_name = "SINF" + this->folder;
            Timer timer(timer_name.c_str());

            VecLargeNum coverage(this->n, 0);
            this->init_RRsets_cov_by_S();

            for (size_t i = 0; i < this->_RRsets.size(); i++) {
                if (!this->_vec_bool_RRset_cov_by_S[i]) {
                    for (auto x : this->_RRsets[i]) {
                        coverage[x]++;
                    }
                }
            }
            // If wanna use heap, please uncomment the following part
            /*
            for (size_t i = 0; i < this->_vec_cand_edges.size(); i++) {
                UVWEdge& edge_tuple = this->_vec_cand_edges[i];
                heap.push(make_pair(i, coverage[edge_tuple.second.first]));
            }
            timer.log_till_now("Initialization", log_file);
            while (!heap.empty() && vec_selected_edges_single_inf.size() < this->args.k_edges) {
                PairIntSizet top = heap.top();
                heap.pop();
                if (coverage[this->_vec_cand_edges[top.first].second.first] > 0) {
                    vec_selected_edges_single_inf.push_back(this->_vec_cand_edges[top.first]);
                    coverage[this->_vec_cand_edges[top.first].second.first] = 0;
                }
            }
            if (vec_selected_edges_single_inf.size() < this->args.k_edges) {
                log_info("--- VITAL: Fail to select edges by single influence ---");
                return;
            }
            */
            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                size_t max_cov = 0;
                size_t max_idx = -1;
                for (size_t j = 0; j < this->_vec_cand_edges.size(); j++) {
                    if (coverage[this->_vec_cand_edges[j].second.first] > max_cov) {
                        max_idx = j;
                        max_cov = coverage[this->_vec_cand_edges[j].second.first];
                    }
                }
                if (max_idx == -1) {
                    log_info("--- VITAL: Fail to select edges by single influence ---");
                    return;
                }

                vec_selected_edges_single_inf.push_back(this->_vec_cand_edges[max_idx]);
                coverage[this->_vec_cand_edges[max_idx].second.first] = 0;
            }
            
            timer.log_total_time(log_file);
            
            this->update_RRsets_cov_by_S(vec_selected_edges_single_inf);
            log_file << "Result by SINF: " << this->comp_inf_cov_by_S() << '\n';
            // Write selected edges
            write_UVWEdges(vec_selected_edges_single_inf, this->_cur_working_folder+"selected_edges_SINF.txt");
            log_info("--- Finish saving the edges selected by single influence ---");
            return;
        }

        void select_edges_by_updated_inf() {
            ASSERT(this->_RRsets.size() > 0);
            std::vector<UVWEdge> vec_selected_edges_updated_inf;
            VecBool bool_cand_edges_selected(this->_vec_cand_edges.size(), false);
            log_info("--- Start selecting by updated influence ---");
            string log_filename = this->_cur_working_folder + "log_UINF.txt";
            ofstream log_file(log_filename);

            string timer_name = "UINF" + this->folder;
            Timer timer(timer_name.c_str());
            VecLargeNum coverage(this->n, 0);
            
            this->init_RRsets_cov_by_S();

            ASSERT(this->_vec_bool_RRset_cov_by_S.size() > 0);
            for (size_t i = 0; i < this->_RRsets.size(); i++) {
                if (!this->_vec_bool_RRset_cov_by_S[i]) {
                    for (auto x : this->_RRsets[i]) {
                        coverage[x]++;
                    }
                }
            }
            // log_info("Building heap");
            /*
            std::priority_queue<PairIntSizet, std::vector<PairIntSizet>, CompareBySecondSizet> heap;
            for (size_t i = 0; i < this->_vec_cand_edges.size(); i++) {
                UVWEdge& edge_tuple = this->_vec_cand_edges[i];
                heap.push(make_pair(i, coverage[edge_tuple.second.first]));             
            }
            log_info("Finish Building heap");
            timer.log_till_now("Initialization", log_file);
            while (!heap.empty() && vec_selected_edges_updated_inf.size() < this->args.k_edges) {
                PairIntSizet top = heap.top();
                heap.pop();

                if (coverage[this->_vec_cand_edges[top.first].second.first] > 0) {
                    if (coverage[this->_vec_cand_edges[top.first].second.first] < top.second) {
                        top.second = coverage[this->_vec_cand_edges[top.first].second.first];
                        heap.push(top);
                        continue;
                    }
                    
                    vec_selected_edges_updated_inf.push_back(this->_vec_cand_edges[top.first]);
                    
                    // update coverage after selecting
                    uint32_t target_node = this->_vec_cand_edges[top.first].second.first;
                    double w = this->_vec_cand_edges[top.first].second.second;
                    const VecLargeNum& target_RRsets = this->_node_to_RRsets[target_node];
                    
                    for (size_t RRset_id : target_RRsets) {
                        double rand_double = dsfmt_gv_genrand_open_close();
                        bool flag_add = false;
                        if (this->_casc_model == IC) flag_add = (rand_double < w);
                        else flag_add = (this->_RRsets[RRset_id].back() == target_node && rand_double < 1 - this->_weighted_in_degree[target_node]);
                        if (flag_add) {
                            for (auto& node : this->_RRsets[RRset_id]) {
                                if (coverage[node] > 0) coverage[node]--;
                            }
                        } else continue;
                    }
                    
                    coverage[target_node] = 0;     // Selected nodes will not be selected again
                }
            }
            if (vec_selected_edges_updated_inf.size() < this->args.k_edges) {
                log_info("--- VITAL: Fail to select edges by updated influence ---");
                return;
            }
            */
            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                size_t max_cov = 0;
                size_t max_idx = -1;
                for (size_t j = 0; j < this->_vec_cand_edges.size(); j++) {
                    if (bool_cand_edges_selected[j]) continue;
                    if (coverage[this->_vec_cand_edges[j].second.first] > max_cov) {
                        max_idx = j;
                        max_cov = coverage[this->_vec_cand_edges[j].second.first];
                    }
                }
                if (max_idx == -1) {
                    log_info("--- VITAL: Fail to select edges by UINF ---");
                    return;
                }
                bool_cand_edges_selected[max_idx] = true;
                vec_selected_edges_updated_inf.push_back(this->_vec_cand_edges[max_idx]);
                
                // update coverage after selecting
                uint32_t target_node = this->_vec_cand_edges[max_idx].second.first;
                double w = this->_vec_cand_edges[max_idx].second.second;
                const VecLargeNum& target_RRsets = this->_node_to_RRsets[target_node];
                
                for (size_t RRset_id : target_RRsets) {
                    double rand_double = dsfmt_gv_genrand_open_close();
                    bool flag_add = false;
                    if (this->_casc_model == IC) flag_add = (rand_double < w);
                    else flag_add = (this->_RRsets[RRset_id].back() == target_node && rand_double < 1 - this->_weighted_in_degree[target_node]);
                    if (flag_add && !this->_vec_bool_RRset_cov_by_S[RRset_id]) {
                        this->_vec_bool_RRset_cov_by_S[RRset_id] = true;
                        for (auto& node : this->_RRsets[RRset_id]) {
                            if (coverage[node] > 0) coverage[node]--;
                        }
                    } else continue;
                }
                
                // coverage[target_node] = 0;     // Selected nodes will not be selected again
            }
            timer.log_total_time(log_file);

            log_file << "Result by UINF: " << this->comp_inf_cov_by_S() << '\n';
            // Write selected edges
            write_UVWEdges(vec_selected_edges_updated_inf, this->_cur_working_folder+"selected_edges_UINF.txt");
            log_info("--- Finish saving the edges selected by updated influence ---");
            return;
        }

        void select_edges_by_AIS_U() {
            ASSERT(this->_RRsets.size() > 0);
            std::vector<UVWEdge> vec_selected_edges_AISU;
            log_info("--- Start selecting by AIS-U ---");
            string log_filename = this->_cur_working_folder + "log_AIS-U.txt";
            ofstream log_file(log_filename);
            
            VecBool bool_cand_edges_selected(this->_vec_cand_edges.size(), false);
            string timer_name = "AIS-U" + this->folder;
            Timer timer(timer_name.c_str());
            VecLargeNum coverage(this->n, 0);
            
            ASSERT(this->_vec_bool_RRset_cov_by_S.size() > 0);
            for (size_t i = 0; i < this->_RRsets.size(); i++) {
                if (!this->_vec_bool_RRset_cov_by_S[i]) {
                    for (auto x : this->_RRsets[i]) {
                        coverage[x]++;
                    }
                }
            }

            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                double max_gain = 0;
                size_t max_idx = -1;
                for (size_t j = 0; j < this->_vec_cand_edges.size(); j++) {
                    if (bool_cand_edges_selected[j]) continue;
                    double cur_gain = this->_vec_cand_edges[j].second.second * coverage[this->_vec_cand_edges[j].second.first];
                    if (cur_gain > max_gain) {
                        max_idx = j;
                        max_gain = cur_gain;
                    }
                }
                if (max_idx == -1) {
                    log_info("--- VITAL: Fail to select edges by AIS-U ---");
                    return;
                }
                
                // coverage[this->_vec_cand_edges[max_idx].second.first] = 0;
                bool_cand_edges_selected[max_idx] = true;
                vec_selected_edges_AISU.push_back(this->_vec_cand_edges[max_idx]);
            }
            timer.log_total_time(log_file);
            
            // Compute Result
            this->init_RRsets_cov_by_S();
            this->update_RRsets_cov_by_S(vec_selected_edges_AISU);
            log_file << "Result by AIS-U: " << this->comp_inf_cov_by_S() << '\n';
            // Write selected edges
            write_UVWEdges(vec_selected_edges_AISU, this->_cur_working_folder+"selected_edges_AIS-U.txt");
            log_info("--- Finish saving the edges selected by AIS-U ---");
            return;
        }

        void select_edges_by_RWGreedy() {
            std::vector<UVWEdge> vec_selected_edges_RWGreedy;
            log_info("--- Start selecting by RWGreedy ---");
            string log_filename = this->_cur_working_folder + "log_RWGreedy.txt";
            ofstream log_file(log_filename);

            VecBool bool_cand_edges_selected(this->_vec_cand_edges.size(), false);
            string timer_name = "RWGreedy" + this->folder;
            Timer timer(timer_name.c_str());

            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                double min_ht = 100000;
                size_t max_idx = -1;
                for (size_t j = 0; j < this->_vec_cand_edges.size(); j++) {
                    if (bool_cand_edges_selected[j]) continue;
                    
                    double cur_ht = this->comp_RWGreedy_ht(this->_vec_cand_edges[j], 1000, 20);
                    
                    if (cur_ht < min_ht) {
                        max_idx = j;
                        min_ht = cur_ht;
                    }
                }
                if (max_idx == -1) {
                    log_info("--- VITAL: Fail to select edges by RWGreedy ---");
                    return;
                }
                log_info(i);
                // coverage[this->_vec_cand_edges[max_idx].second.first] = 0;
                bool_cand_edges_selected[max_idx] = true;
                vec_selected_edges_RWGreedy.push_back(this->_vec_cand_edges[max_idx]);
                this->add_edge(this->_vec_cand_edges[max_idx]);
            }
            timer.log_total_time(log_file);
            
            // Write selected edges
            write_UVWEdges(vec_selected_edges_RWGreedy, this->_cur_working_folder+"selected_edges_RWGreedy.txt");
            log_info("--- Finish saving the edges selected by RWGreedy ---");
            return;
        }

        void IMA() {
            ASSERT(this->_seed_set_to_augment.size() > 0);
            ASSERT(this->_cur_RRsets_num > 0);

            string log_filename = this->_cur_working_folder + "log_AIS.txt";
            ofstream log_file(log_filename);

            Timer timer("AIS");

            log_info("--- Initializing the marginal coverage array ---");
            
            this->init_seed_cov();
            log_file << "Original Inf: " << this->comp_inf_cov_by_S() << '\n';
            
            timer.log_operation_time("Init seed cov", log_file);
            log_info("--- Start Reading Candidate Edges ---");
            this->read_cand_edges();
            // log_info("--- Start Computing the marginal gains of the edges ---");
            // this->comp_edges_gain();
            this->_vec_bool_cand_edges_selected.resize(this->_vec_cand_edges.size(), false);
            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                size_t best_idx = this->get_idx_max_marginal_gain();
                this->_vec_selected_edges.push_back(this->_vec_cand_edges[best_idx]);
                this->_vec_bool_cand_edges_selected[best_idx] = true;
                
                // update the RR sets
                UVWEdge& added_edge = this->_vec_cand_edges[best_idx];
                uint32_t u = added_edge.first, v = added_edge.second.first;
                double w = added_edge.second.second;
                this->augment_RRsets_early_stop(u, v, w);
            }
            timer.log_total_time(log_file);
            
            string edges_save_path = this->_cur_working_folder+"selected_edges_AIS.txt";
            write_UVWEdges(this->_vec_selected_edges, edges_save_path);
            log_file << "Result by AIS: " << this->comp_inf_cov_by_S() << '\n';
            log_file << "Result by AISPAC_est: " << this->estimate_by_pmc(this->_vec_selected_edges) << '\n';
            
            return;
        }

        void AIS(string log_filepath="") {
            ASSERT(this->_seed_set_to_augment.size() > 0);
            ASSERT(this->_cur_RRsets_num > 0);

            // string log_filename = this->_cur_working_folder + "log_IMA.txt";
            log_info("--- Start Reading Candidate Edges ---");
            this->read_cand_edges();

            if (log_filepath.size() == 0) {
                log_filepath = this->_cur_working_folder + "log_AIS.txt";
            }
            ofstream log_file(log_filepath, std::ios::app);
            // ofstream log_file(log_filepath);

            Timer timer("AIS");
            log_info("--- Initializing the marginal coverage array ---");
            
            this->init_seed_cov();
            log_file << "Original Inf: " << this->comp_inf_cov_by_S() << '\n';
            
            timer.log_operation_time("Init seed cov", log_file);
            
            // log_info("--- Start Computing the marginal gains of the edges ---");
            // this->comp_edges_gain();
            this->_vec_bool_cand_edges_selected.resize(this->_vec_cand_edges.size(), false);

            for (uint32_t i = 0; i < this->args.k_edges; i++) {
                size_t best_idx = this->get_idx_max_marginal_gain();
                this->_vec_selected_edges.push_back(this->_vec_cand_edges[best_idx]);
                this->_vec_bool_cand_edges_selected[best_idx] = true;
                
                // update the RR sets
                UVWEdge& added_edge = this->_vec_cand_edges[best_idx];
                uint32_t u = added_edge.first, v = added_edge.second.first;
                double w = added_edge.second.second;
                this->augment_RRsets_early_stop(u, v, w);
            }
            timer.log_operation_time("Selection", log_file);
            
            string edges_save_path = this->_cur_working_folder+"selected_edges_AIS.txt";

            write_UVWEdges(this->_vec_selected_edges, edges_save_path);
            log_file << "Result by AIS: " << this->comp_inf_cov_by_S() << '\n';
            log_file << "Result by AISPAC_est: " << this->estimate_by_pmc(this->_vec_selected_edges) << '\n';
            
            return;
        }

        void select_edges_by_AISPAC(string log_filepath="") {
            ASSERT(this->_seed_set_to_augment.size() > 0);
            ASSERT(this->_cur_RRsets_num > 0);
            // preparation

            if (log_filepath.size() == 0) {
                log_filepath = this->_cur_working_folder + "log_AISPAC.txt";
            }
            ofstream log_file(log_filepath, std::ios::app);
            
            Timer timer("AISPAC");

            log_info("--- Initializing coverage boolean arrays ---");
            
            this->init_seed_cov();
            log_file << "Original Inf: " << this->comp_inf_cov_by_S() << '\n';

            this->_vec_node_prob_heap.resize(this->n);
            
            log_info("--- Start Reading Candidate Edges ---");
            this->read_cand_edges();

            // Traverse cand_edges and organize probability
            for (size_t i = 0; i < this->_vec_cand_edges.size(); i++) {
                uint32_t v = this->_vec_cand_edges[i].second.first;
                double puv = this->_vec_cand_edges[i].second.second;
                this->_vec_node_prob_heap[v].push(make_pair(i, puv));
            }
            log_info("Finish organizing prob");
            
            // optimize PMC
            double res_inf = this->edge_selection_with_PMC(this->args.k_edges);
            timer.log_operation_time("Selection", log_file);
            
            // self-evaluate
            this->update_RRsets_cov_by_S(this->_vec_selected_edges);
            
            // write resulting edges
            
            string edges_save_path = this->_cur_working_folder+"selected_edges_AISPAC.txt";
            write_UVWEdges(this->_vec_selected_edges, edges_save_path);
            // log_file << "Result by AISPAC: " << res_inf << '\n';
            log_file << "Result by AISPAC_est: " << this->estimate_by_pmc(this->_vec_selected_edges) << '\n';
            return;
        }

        void select_edges_by_AugIM(string log_filepath="") {
            ASSERT(this->_seed_set_to_augment.size() > 0);
            ASSERT(this->_cur_RRsets_num > 0);
            ASSERT(this->_vec_cand_edges.size() > 0);
            if (log_filepath.size() == 0) {
                log_filepath = this->_cur_working_folder + "log_AugIM.txt";
            }
            ofstream log_file(log_filepath, std::ios::app);
            
            Timer timer("AugIM");
            
            log_info("--- Initializing coverage boolean arrays ---");
            this->init_seed_cov();
            log_file << "Original Inf: " << this->comp_inf_cov_by_S() << '\n';

            VecVecLargeNum RRsets_for_edges(this->_cur_RRsets_num);
            VecVecLargeNum edge_to_RRsets(this->_vec_cand_edges.size());

            // For every edge, initialize its RR sets. Re-organize RR sets for edges
            int cnt = 0, all_cnt = 0;
            for (size_t i=0; i<this->_vec_cand_edges.size(); i++)
            {
                auto& cand_edge = this->_vec_cand_edges[i];
                uint32_t u = cand_edge.first, v = cand_edge.second.first;
                double puv = cand_edge.second.second;
                for (size_t RR_idx : this->_node_to_RRsets[v]) {
                    // all_cnt++;
                    if (!this->_vec_bool_RRset_cov_by_S[RR_idx] && dsfmt_gv_genrand_open_close() < puv) {
                        RRsets_for_edges[RR_idx].push_back(i);
                        edge_to_RRsets[i].push_back(RR_idx);
                        // cnt++;
                    }
                }
            }
            timer.log_operation_time("Sampling 2", log_file);
            // Selection
            VecLargeNum vec_selected_idx = max_cover_by_heap(edge_to_RRsets, RRsets_for_edges, this->args.k_edges);
            timer.log_operation_time("Selection", log_file);
            
            for (auto i : vec_selected_idx) {
                this->_vec_selected_edges.push_back(this->_vec_cand_edges[i]);
            }
            string edges_save_path = this->_cur_working_folder+"selected_edges_AugIM.txt";
            write_UVWEdges(this->_vec_selected_edges, edges_save_path);
            log_file << "Result by AISPAC_est: " << this->estimate_by_pmc(this->_vec_selected_edges) << '\n';
            return;
        }

        size_t count_RRsets_with_size(size_t num) {
            size_t res = 0;
            for (auto &RRset: this->_RRsets) {
                if (RRset.size() == num) res++;
            }
            return res;
        }
        void create_param_dir() {
            if (!check_file_exist(this->folder + "params/")) {
                log_info("--- Creating params folder ---");
                make_dir(this->folder + "params/");
            }

            string cur_folder_name = this->folder + "params/" + to_string(this->args.k_edges) + "_" + to_string(this->args.num_cand_edges) + "_" + to_string(this->args.epsilon) + "_" + to_string(this->args.delta) + "_" + to_string(this->args.rand_seed) + "_" + this->_prob_mode;
            if (this->_seed_mode != "IM")
                cur_folder_name = this->folder + "params/" + to_string(this->args.k_edges) + "_" + to_string(this->args.num_cand_edges) + "_" + to_string(this->args.epsilon) + "_" + to_string(this->args.delta) + "_" + to_string(this->args.rand_seed) + "_" + this->_seed_mode + "seed_" + this->_prob_mode;
            if (this->args.beta > 1) {
                cur_folder_name = cur_folder_name + "_beta_" + to_string(this->args.beta);
            }
            if (this->args.num_samples != 0) {
                cur_folder_name = cur_folder_name + "_numsamples_" + to_string(this->args.num_samples);
            }
            bool flag = make_dir(cur_folder_name);
            if (flag)
            {
                log_info("--- Current experiments' data will be stored in " + cur_folder_name + " ---");
                this->_cur_working_folder = cur_folder_name + "/";
            }
            else log_info("--- Failed to create the dir ---");
            return;
        }
};
#endif