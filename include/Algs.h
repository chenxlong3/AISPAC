#ifndef ALGS_H
#define ALGS_H
#include "InfGraph.h"
class IMM {
    private:
        static double step1(InfGraph& g, const Argument& args) {
            double eps_prime = 0.3 * sqrt(2);
            for (uint32_t x=1; ; x++) {
                // size_t ci = (2+2/3 * eps_prime)* ( log(g.n) + logcnk(g.n, args.k) + log(log2(g.n))) * pow(2.0, x) / (eps_prime*eps_prime);
                size_t ci = (2+2/3 * eps_prime)* ( log(g.n) + logcnk(g.n, args.k_seed) + log(log2(g.n))) * pow(2.0, x) / (eps_prime*eps_prime);
                // std::cout << "Number of RR sets: " << ci << std::endl;
                g.build_RRsets(ci);
                double ept = g.node_selection(args.k_seed);
                if (ept > 1.0 / pow(2.0, x)) {
                    double OPT_prime = ept * g.n / (1+eps_prime);
                    std::cout << "The influence estimated by ept*n: " << ept*g.n << std::endl;
                    
                    return OPT_prime;
                }
            }
            return -1.0;
        }
        static double step2(InfGraph& g, const Argument& args, double OPT_prime) {
            ASSERT(OPT_prime > 0);
            double e = exp(1);
            double alpha = sqrt(log(g.n) + log(2));
            double beta = sqrt((1-1/e) * (logcnk(g.n, args.k_seed) + log(g.n) + log(2)));
            size_t theta = 2.0 * g.n * pow2((1-1/e) * alpha + beta) / OPT_prime / 0.3 / 0.3;
            g.build_RRsets(theta);
            double opt = g.node_selection(args.k_seed)*g.n;
            return opt;
        }
        
    public:
        static void influence_maximize(InfGraph &g, const Argument &args) {
            double OPT_prime = step1(g, args);
            double OPT_star = step2(g, args, OPT_prime);
            std::cout << "The influence estimated at the end of IMM: " << OPT_star << std::endl;
            return;
        }
};

class TIM {
public:
    Argument args;
    // InfGraph g;
    // TIM(InfGraph &g, Argument& args) {
    //     this->g = g;
    //     this->args = args;
    // }
    double algo2(InfGraph &g) {
        double LB = 1/2.0, c = 0;
        while (true)
        {
            size_t loop = (6*log(g.n) + 6*log(log(g.n)/log(2))) / LB;
            double sum_RRset_width = 0;
            for (size_t i = 0; i < loop; i++)
            {
                uint32_t rand_node = dsfmt_gv_genrand_uint32_range(g.n);
                double w_R = (double)g.build_one_RRset(rand_node, 0);
                double tmp = w_R / g.m;
                sum_RRset_width += w_R;
                c += 1 - pow(1-tmp, this->args.k_seed);
            }
            c /= loop;
            if (c>LB) break;
            LB /= 2;
        }
        return c*g.n;
    }
    double Kpt_estimation(InfGraph &g)
    {
        double ept=algo2(g);
        ept/=2;
        return ept;
    }
    void refined_KPT (InfGraph g, double ept) {
        size_t theta = (2 + args.epsilon) * ( g.n * log(g.n) ) / ( args.epsilon * args.epsilon * ept);
    }
    void Two_phase_IM(InfGraph &g) {
        return;
    }
};

class IEM {
public:
    static void generate_RRsets_for_estimate(InfGraph& g) {
        double eps_over_k = g.args.epsilon / (double)g.args.k_edges;
        double lambda = eps_over_k / (2 + eps_over_k);
        double eps = sqrt(2)*lambda;
        double delta = g.args.delta;
        uint32_t k_seed = g._seed_set_to_augment.size();
        
        ASSERT(k_seed > 0);
        double sigma_S = k_seed;
        for (uint32_t x=1; ; x++) {
            size_t ci = (2+2/3 * eps)*log(1.0/delta) * pow(2.0, x) / (eps*eps) / g.args.beta;
            g.build_RRsets(ci, g._truncated_or_not);
            double fr_S = g.comp_inf_by_cov(g._seed_set_to_augment) / g.n;
            std::cout << fr_S << ' ' << 1.0 / pow(2.0, x) << std::endl;
            if (fr_S > 1.0 / pow(2.0, x)) {
                sigma_S = fr_S * g.n / (1+eps);
                std::cout << "The estimated influence: " << sigma_S << std::endl;
                break;
            }
        }
        // size_t theta = (2+2/3 * eps)*log(1/delta)*g.n / (eps*eps*sigma_S);
        // g.build_RRsets(theta);
        return;
    }
    static void generate_RRsets(InfGraph& g) {
        double eps_over_k = g.args.epsilon / (double)g.args.k_edges;
        double lambda = eps_over_k / (2 + eps_over_k);
        double eps = lambda;
        if (g._vec_cand_edges.size() == 0) {
            log_info("VITAL: Please load the candidate edges first.");
            return;
        }
        uint32_t min_cand_n = (g.n < g._vec_cand_edges.size()) ? g.n : g._vec_cand_edges.size();
        double delta = g.args.delta / (4.0*g.args.k_edges*min_cand_n);
        uint32_t beta = g.args.beta;
        uint32_t k_seed = g._seed_set_to_augment.size();
        ASSERT(k_seed > 0);
        double sigma_S = k_seed;
        for (uint32_t x=1; ; x++) {
            size_t ci = (2+2/3 * eps)*log(1.0/delta) * pow(2.0, x) / (eps*eps*beta);
            g.build_RRsets(ci, g._truncated_or_not);
            double fr_S = g.comp_inf_by_cov(g._seed_set_to_augment) / g.n;
            std::cout << fr_S << ' ' << 1.0 / pow(2.0, x) << std::endl;
            if (fr_S > 1.0 / pow(2.0, x)) {
                sigma_S = fr_S * g.n / (1+eps);
                std::cout << "The estimated influence: " << sigma_S << std::endl;
                break;
            }
        }
        size_t theta = (2+2/3 * eps)*log(1/delta)*g.n / (eps*eps*beta*sigma_S);
        g.build_RRsets(theta);
        return;
    }
};

class IE {
    public:
        static double stopping_rules(InfGraph& g, bool error_divide=true) {
            double eps_over_k = g.args.epsilon / (double)g.args.k_edges;
            if (!error_divide)  eps_over_k = g.args.epsilon;
            double eps = eps_over_k / (2 + eps_over_k);
            if (g._vec_cand_edges.size() == 0) {
                log_info("VITAL: Please load the candidate edges first.");
                return 0.0;
            }
            uint32_t min_cand_n = (g.n < g._vec_cand_edges.size()) ? g.n : g._vec_cand_edges.size();
            double delta = g.args.delta / (g.args.k_edges*min_cand_n);
            uint32_t beta = g.args.beta;
            uint32_t k_seed = g._seed_set_to_augment.size();
            ASSERT(k_seed > 0);

            double Gamma = 2*(1 + eps)*(1 + eps/3)*log(2.0 / delta) / (eps*eps*beta);
            if (!error_divide) {
                Gamma = 2 * (1 + eps) * (1 + eps / 3) * (log(2.0 / delta) + logcnk(min_cand_n, g.args.k_edges)) / (eps * eps * beta);
            }

            size_t theta=0;
            double Sigma=0.0;
            log_info("Gamma", Gamma);
            while (Sigma < Gamma) {
                uint32_t rand_node = dsfmt_gv_genrand_uint32_range(g.n);
                // this->_node_sampled_times[rand_node]++;
                
                bool end_flag = g.build_one_RRset(rand_node, g._cur_RRsets_num, false, g._truncated_or_not);
                
                g._cur_RRsets_num++;
                if (end_flag) {
                    Sigma++;
                }
                theta++;
            }
            double est_inf = g.n * Gamma / theta;
            log_info("Total Number of RR sets", theta);
            log_info("Estimated Influence", est_inf);
            return est_inf;
        }
};

class MAXCOV {
public:
    VecLargeNum max_cover_lazy(VecVecLargeNum vec_u, VecVecLargeNum vec_v, uint32_t target_size) {
        size_t num_u = vec_u.size();
        size_t num_v = vec_v.size();
        VecLargeNum res;
        VecLargeNum coverage(vec_u.size(), 0);
        size_t max_cov = 0;
        size_t sum_cov = 0;
        for (auto i = num_u; i--;)
        {
            const auto deg = vec_u[i].size();
            coverage[i] = deg;
            if (deg > max_cov) max_cov = deg;
        }
        VecVecLargeNum deg_map(max_cov + 1); // deg_map: map degree to the nodes with this degree
        for (auto i = num_u; i--;)
        {
            if (coverage[i] == 0) continue;
            deg_map[coverage[i]].push_back(i);
        }
        VecBool lower_nodes_mark(num_v, false);
        for (auto deg = max_cov; deg > 0; deg--) {
            auto& vec_nodes = deg_map[deg];
            for (auto idx = vec_nodes.size(); idx--;) {
                auto argmax_idx = vec_nodes[idx];
                const auto cur_cov = coverage[argmax_idx];
                if (deg > cur_cov) {
                    deg_map[cur_cov].push_back(argmax_idx);
                    continue;
                }
                auto topk = target_size;
				auto deg_bound = deg;
				VecLargeNum vecBound(target_size);
                
                // Initialize vecBound
				auto idx_bound = idx + 1;
				while (topk && idx_bound--)
				{
					vecBound[--topk] = coverage[deg_map[deg_bound][idx_bound]];
				}
				while (topk && --deg_bound)
				{
					idx_bound = deg_map[deg_bound].size();
					while (topk && idx_bound--)
					{
						vecBound[--topk] = coverage[deg_map[deg_bound][idx_bound]];
					}
				}
				make_min_heap(vecBound);

                // Find the top-k marginal coverage
				auto flag = topk == 0;
				while (flag && idx_bound--)
				{
					const auto cur_deg_bound = coverage[deg_map[deg_bound][idx_bound]];
					if (vecBound[0] >= deg_bound)
					{
						flag = false;
					}
					else if (vecBound[0] < cur_deg_bound)
					{
						min_heap_replace_min_value(vecBound, cur_deg_bound);
					}
				}
                while (flag && --deg_bound)
				{
					idx_bound = deg_map[deg_bound].size();
					while (flag && idx_bound--)
					{
						const auto currdeg_bound = coverage[deg_map[deg_bound][idx_bound]];
						if (vecBound[0] >= deg_bound)
						{
							flag = false;
						}
						else if (vecBound[0] < currdeg_bound)
						{
							min_heap_replace_min_value(vecBound, currdeg_bound);
						}
					}
				}
            }
        }
        return res;
    }

    
};

#endif