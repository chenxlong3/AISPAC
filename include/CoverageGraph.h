#ifndef COVERAGE_GRAPH_H
#define COVERAGE_GRAPH_H
#include "headers.h"
class CoverageGraph
{
private:
    /* data */
    uint32_t _num_V;
	/// __numE: number of edges in the graph.
	size_t _num_E;
	/// __numRRsets: number of RR sets.
	size_t _num_RRsets = 0;
    
public:
    CoverageGraph(/* args */) {

    }
    ~CoverageGraph() {

    }
    /// Initialization
	
    double comp_inf(const NodeList& vec_seed) {
        std::deque<bool> vec_bool_visit(this->_num_RRsets);
        std::deque<bool> vec_bool_seed(this->_num_RRsets);
        // for(auto seed: vec_seed) 
    }

};



#endif