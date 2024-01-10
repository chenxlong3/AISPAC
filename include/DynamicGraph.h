#ifndef DYNAMIC_GRAPH_H
#define DYNAMIC_GRAPH_H
#include "Algs.h"
#include "InfGraph.h"
class DynamicGraph : public InfGraph {
   public:
    VecVecInt32 _RRsets_est, _RRsets_cov;
    VecVecLargeNum _node_to_RRsets_est, _node_to_RRsets_cov;
    VecLargeNum _OPT_threads;
    size_t _num_threads;
    VecSetInt _SET_RRsets_est, _SET_RRsets_cov;
    size_t _cur_RRsets_num_est = 0, _cur_RRsets_num_cov = 0;
    LL _number_trav_edges_est = 0;
    uint32_t n_0, m_0; // The initial number of nodes and edges in the current iteration.

    Argument dyn_args = Argument(this->args.k, this->args.epsilon/2);
    double _p;
    DynamicGraph(string filename) : InfGraph(filename) {
        this->n_0 = this->n;
        this->m_0 = this->m;
    }
    
    void empty_init() {
        // Clear vectors in Dynamic Graph
        VecVecInt32().swap(this->_RRsets_est);
        VecVecInt32().swap(this->_RRsets_cov);
        VecVecLargeNum().swap(this->_node_to_RRsets_est);
        VecVecLargeNum().swap(this->_node_to_RRsets_cov);
        this->_cur_RRsets_num_est = 0;
        this->_cur_RRsets_num_cov = 0;
        this->_number_trav_edges_est = 0;
        // Clear the properties in InfGraph
        this->clean_RRsets_InfGraph();
        return;
    }
    void test() {
        IMM::influence_maximize(*this, this->args);
    }
    void build_graph() {
        this->empty_init();
        IMM::influence_maximize(*this, this->args);
        this->estimate_num_RRsets_per_node();
        this->dyn_build_RRsets_with_p(this->_p, EST);
        this->dyn_build_RRsets_with_p(this->_p, COV);
        return;
    }

    void dyn_build_RRsets_with_p(double p, RRsetMode mode = EST) {
        size_t total_num = std::ceil(p * this->n);
        if (total_num > SIZE_MAX) {
            std::cout << "Error: The number of RR sets is too large." << std::endl;
            exit(1);
        }
        total_num = 0;
        for (auto i = 0; i < this->n; i++) {
            this->dyn_build_RRsets_one_node_p(i, p, mode);
        }
        return;
    }

    void dyn_build_RRsets_one_node_p(uint32_t v, double p, RRsetMode mode = EST) {
        // Input:
        //     v: the target node;
        //     p: the expected number of RR sets to sample
        //     mode: EST or COV
        // This function is to sample RR sets for one node with prob. p

        uint32_t int_part = std::floor(p);
        const double float_part = p - int_part;
        ASSERT(float_part <= 1.0);

        double rand_double = dsfmt_gv_genrand_open_close();
        if (rand_double <= float_part) int_part += 1;
        const size_t prev_size = (mode == EST) ? this->_cur_RRsets_num_est : this->_cur_RRsets_num_cov;
        if (mode == EST) {
            this->_cur_RRsets_num_est += int_part;
        } else if (mode == COV) {
            this->_cur_RRsets_num_cov += int_part;
        }
        for (auto j = 0; j < int_part; j++) {
            this->dyn_build_one_RRset(v, prev_size + j, mode);
        }
    }

    void dyn_build_one_RRset(uint32_t v, const size_t RRset_idx, RRsetMode mode = EST) {
        NodeList vec_vis_nodes;  // The list of visited nodes
        SetInt32 set_vis_nodes;
        size_t num_visit_node = 0, cur_idx = 0;  // Number of activated nodes; Current traversal index
        LL num_trav_edges = 0;
        // For v, the target node
        this->update_node_to_RRsets(v, RRset_idx, mode);

        vec_vis_nodes.push_back(v);
        set_vis_nodes.insert(v);
        num_visit_node++;
        this->_vec_bool_vis[v] = true;

        while (cur_idx < num_visit_node) {      // If no new nodes show up
            const auto expand = vec_vis_nodes[cur_idx++];
            num_trav_edges += this->_inv_G[expand].size();
            if (this->_casc_model == IC) {
                for (auto& nbr : this->_inv_G[expand]) {
                    const auto nbr_id = nbr.first;
                    if (this->_vec_bool_vis[nbr_id]) {
                        // if(num_visit_node == 1) std::cout << "Skip...";
                        continue;
                    }
                    const auto rand_double = dsfmt_gv_genrand_open_close();
                    if (rand_double <= nbr.second) {
                        // The node nbr is activated. Add it into the vector and set.
                        vec_vis_nodes.push_back(nbr_id);
                        set_vis_nodes.insert(nbr_id);
                        num_visit_node++;
                        this->_vec_bool_vis[nbr_id] = true;
                        this->update_node_to_RRsets(nbr_id, RRset_idx, mode);
                    }
                }
            } else if (this->_casc_model == LT) {
                if (this->_inv_G[expand].size() == 0) break;
                num_trav_edges += this->_inv_G[expand].size();
                const auto next_nbr_idx = gen_rand_node_by_weight_LT(this->_inv_G[expand]);
                if (next_nbr_idx >= this->_inv_G[expand].size()) break;
                const auto nbr_id = this->_inv_G[expand][next_nbr_idx].first;
                if (this->_vec_bool_vis[nbr_id]) break;

                vec_vis_nodes.push_back(nbr_id);
                set_vis_nodes.insert(nbr_id);
                num_visit_node++;
                this->_vec_bool_vis[nbr_id] = true;
                this->update_node_to_RRsets(nbr_id, RRset_idx, mode);
            }
        }

        ASSERT(num_visit_node == (size_t)vec_vis_nodes.size());
        // std::cout << vec_vis_nodes.size() << std::endl;
        // print_vector(vec_vis_nodes);
        // Clean _vec_bool_vis
        for (auto i = 0; i < num_visit_node; i++) this->_vec_bool_vis[vec_vis_nodes[i]] = false;

        if (mode == EST) {
            this->_RRsets_est.push_back(vec_vis_nodes);
            this->_SET_RRsets_est.push_back(set_vis_nodes);
            this->_number_trav_edges_est += num_trav_edges;
        } else if (mode == COV) {
            this->_RRsets_cov.push_back(vec_vis_nodes);
            this->_SET_RRsets_cov.push_back(set_vis_nodes);
        }
        return;
    }

    void estimate_num_RRsets_per_node() {
        this->_p = (double)this->_cur_RRsets_num / this->n;
        return;
    }

    void add_node_to_RRset_est(uint32_t u, size_t RRset_idx) {
        this->_RRsets_est[RRset_idx].push_back(u);
        this->_SET_RRsets_est[RRset_idx].insert(u);
        return;
    }

    void add_node_to_RRset_cov(uint32_t u, size_t RRset_idx) {
        this->_RRsets_cov[RRset_idx].push_back(u);
        this->_SET_RRsets_cov[RRset_idx].insert(u);
        return;
    }

    void update_node_to_RRsets(uint32_t u, size_t RRset_idx, RRsetMode mode=EST) {
        if (mode == EST)
            this->_node_to_RRsets_est[u].push_back(RRset_idx);
        else
            this->_node_to_RRsets_cov[u].push_back(RRset_idx);
        return;
    }

    bool aug_insert_node_to_RRset_est(uint32_t u, size_t RRset_idx) {
        if (this->_SET_RRsets_est[RRset_idx].count(u) == 0)
        {
            this->_RRsets_est[RRset_idx].push_back(u);
            this->_SET_RRsets_est[RRset_idx].insert(u);
            this->_node_to_RRsets_est[u].push_back(RRset_idx);
            return true;
        }
        return false;
    }

    bool aug_insert_node_to_RRset_cov(uint32_t u, size_t RRset_idx) {
        if (this->_SET_RRsets_cov[RRset_idx].count(u) == 0)
        {
            this->_RRsets_cov[RRset_idx].push_back(u);
            this->_SET_RRsets_cov[RRset_idx].insert(u);
            this->_node_to_RRsets_cov[u].push_back(RRset_idx);
            return true;
        }
        return false;
    }

    void augment_one_RRset(uint32_t u, size_t RRset_idx, RRsetMode mode = EST) {
        ASSERT(mode == EST || mode == COV);
        std::queue<uint32_t> Q;
        Q.push(u);

        while (!Q.empty()) {
            uint32_t node_id = Q.front();
            Q.pop();
            if (mode == EST) {
                this->_num_traversed_edges += this->_inv_G[node_id].size();
            }

            if (this->_casc_model == IC) {
                for (auto& nbr : this->_inv_G[node_id]) {
                    double rand_double = dsfmt_gv_genrand_open_close();
                    bool flag_push_to_q;
                    if (rand_double <= nbr.second) {
                        if (mode == EST) {
                            flag_push_to_q = this->aug_insert_node_to_RRset_est(nbr.first, RRset_idx);
                        } else if (mode == COV) {
                            flag_push_to_q = this->aug_insert_node_to_RRset_cov(nbr.first, RRset_idx);
                        }
                        if (flag_push_to_q) Q.push(nbr.first);
                    }
                }
            } else if (this->_casc_model == LT) {
                if (this->_inv_G[node_id].size() == 0) break;
                const auto next_nbr_idx = gen_rand_node_by_weight_LT(this->_inv_G[node_id]);
                if (next_nbr_idx >= this->_inv_G[node_id].size()) break;
                const auto nbr_id = this->_inv_G[node_id][next_nbr_idx].first;
                bool flag_push_to_q = (mode == COV) ? this->aug_insert_node_to_RRset_est(nbr_id, RRset_idx) : this->aug_insert_node_to_RRset_cov(nbr_id, RRset_idx);
                if (flag_push_to_q) {
                    Q.push(nbr_id);
                }
            }
        }
        return;
    }

    void augment_RRsets(uint32_t u, uint32_t v, double w, RRsetMode mode = EST) {
        const VecLargeNum& target_RRsets = (mode == EST) ? this->_node_to_RRsets_est[v] : this->_node_to_RRsets_cov[v];
        // How to check whether a node is already in a RR set
        for (size_t RRset_id : target_RRsets) {
            double rand_double = dsfmt_gv_genrand_open_close();
            if (rand_double < w) {
                this->augment_one_RRset(u, RRset_id, mode);
            } else  continue;
        }

        return;
    }

    void add_edge(uint32_t u, uint32_t v, double w) {
        this->_G[u].push_back(Edge(v, w));
        this->_inv_G[v].push_back(Edge(u, w));
        this->m++;
        if (this->m >= 2 * this->m_0) {
            this->m_0 = this->m;
            this->build_graph();
            return;
        }
        const auto prev_AVG = 1.0 * this->_number_trav_edges_est / this->_cur_RRsets_num_est;
        this->augment_RRsets(u, v, w, EST);
        double cur_AVG = 1.0 * this->_number_trav_edges_est / this->_cur_RRsets_num_est;
        if (cur_AVG >= 2 * prev_AVG) {
            this->n_0 = this->n; this->m_0 = this->m;
            this->build_graph();
            return;
        }
        this->augment_RRsets(u, v, w, COV);
        return;
    }

    void add_node(uint32_t u) {
        this->_G.push_back(EdgeList());
        this->_inv_G.push_back(EdgeList());
        this->n++;
        if (this->n >= 2 * this->n_0) {
            this->n_0 = this->n;
            this->build_graph();
            return;
        }
        this->dyn_build_RRsets_one_node_p(u, this->_p);
        return;
    }

    void insert_edge_cov(uint32_t v, size_t RRset_idx) {
        
        
        return;
    }
};
#endif