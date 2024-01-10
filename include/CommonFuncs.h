#ifndef COMMON_FUNCS_H
#define COMMON_FUNCS_H
#include "headers.h"
template <typename _Ty>
static inline void log_info(_Ty val) {
    std::cout << val << std::endl;
}

/// Log information
template <typename _Ty>
static inline void log_info(const std::string title, _Ty val) {
    std::cout << title << ": " << val << std::endl;
}

/// Math, pow2
static inline double pow2(const double t) {
    return t * t;
}

/// Math, log2
static inline double log2(const size_t n) {
    return log(n) / log(2);
}

/// Math, logcnk
static inline double logcnk(const size_t n, size_t k) {
    k = k < n - k ? k : n - k;
    double res = 0;
    for (auto i = 1; i <= k; i++) res += log(double(n - k + i) / i);
    return res;
}

void print_vector(std::vector<uint32_t> v) {
    for(auto node_id: v) {
        std::cout << node_id << ' ';
    }
    std::cout << '\n';
    return;
}

void print_vector(std::vector<size_t> v) {
    for(auto node_id: v) {
        std::cout << node_id << ' ';
    }
    std::cout << '\n';
    return;
}

void print_vector(VecDouble v) {
    for(auto x: v) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
    return;
}

void print_vector(std::vector<UVWEdge> v) {
    for(auto& tuple: v) {
        std::cout << tuple.first << ' ' << tuple.second.first << ' ' << tuple.second.second << std::endl;
    }
    std::cout << '\n';
    return;
}

void print_vector(std::vector<uint32_t> v, uint32_t max_len) {
    if (max_len > v.size()) max_len = v.size();
    for(auto i=0; i<max_len; i++) {
        std::cout << v[i] << ' ';
    }
    std::cout << '\n';
    return;
}


static size_t gen_rand_node_by_weight_LT(const EdgeList& edges) {
    const double weight = dsfmt_gv_genrand_open_close();
    size_t min_idx = 0, max_idx = edges.size() - 1;
    if (weight < edges.front().second) return 0;
    if (weight > edges[max_idx].second) return edges.size();
    while (max_idx > min_idx) {
        const size_t mid_idx = (min_idx + max_idx) / 2;
        const auto mid_weight = edges[mid_idx].second;
        if (weight <= mid_weight) max_idx = mid_idx;
        else min_idx = mid_idx+1;
    }
    return max_idx;
}

template <typename T>
double comp_mean(std::vector<T> v) {
    return (double)std::accumulate(v.begin(), v.end(), 0.0) / v.size();
}

double variance(VecLargeNum a, int n) {
    // Compute mean (average of elements)
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    double mean = (double)sum /
                  (double)n;

    // Compute sum squared
    // differences with mean.
    double sqDiff = 0;
    for (int i = 0; i < n; i++)
        sqDiff += (a[i] - mean) *
                  (a[i] - mean);
    return sqDiff / n;
}

std::vector<UVWEdge> read_UVWEdges(string filename) {
    uint32_t u, v;
    double w;
    ifstream myfile(filename);
    std::vector<UVWEdge> res;
    while (myfile >> u >> v >> w) {
        res.push_back(make_pair(u, Edge(v, w)));
    }
    myfile.close();
    return res;
}

void write_UVWEdges(std::vector<UVWEdge>& tuple_list, string filename) {
    ofstream outfile(filename);
    for (auto& tuple : tuple_list) {
        outfile << tuple.first << '\t' << tuple.second.first << '\t' << tuple.second.second << '\n';
    }
    outfile.close();
    log_info("Finish saving the edges to " + filename);
    return;
}

void ExitMessage(string msg) {
    cout << msg << endl;
    exit(1);
}

bool make_dir(string dir_path) {
    bool ret = false;
    DIR* cur_dir = nullptr;
    cur_dir = opendir(dir_path.c_str());
    if (cur_dir == NULL)
    {
        log_info("--- This path does not exist, creating ---");
        ret = (mkdir(dir_path.c_str(), 0777) == 0);
        if (!ret)   log_info("--- mkdir FAILED ---");
        else log_info("--- mkdir success ---");
    }
    else {
        ret = true;
        log_info("--- This dir exists ---");
        closedir(cur_dir);
    }
    return ret;
}

void save_vector(NodeList v, string out_file) {
    ofstream myfile(out_file);
    for (auto& i : v) {
        myfile << i << '\n';
    }
    return;
    
}

bool check_file_exist(string file_path) {
    ifstream ifile;
    ifile.open(file_path);
    if(ifile) {
        ifile.close();
        return true;
    } else {
        return false;
    }
}
#endif