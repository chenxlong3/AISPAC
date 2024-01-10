#ifndef HEADERS_H
#define HEADERS_H
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <deque>
#include <unordered_map>
#include <boost/dynamic_bitset.hpp>
#include <set>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

#define DSFMT_MEXP 19937
#include "SFMT/dSFMT/dSFMT.h"

#if !defined(DSFMT_MEXP)
#ifdef __GNUC__
#define DSFMT_MEXP 19937
#endif
#endif
#define ASSERT(v) {if (!(v)) {cerr<<"ASSERT FAIL @ "<<__FILE__<<":"<<__LINE__<<endl; exit(1);}}
using namespace std;
typedef long long LL;
typedef std::set<uint32_t> SetInt32;
typedef std::set<size_t> SetSizet;
typedef std::vector<uint32_t> NodeList;
typedef std::vector<uint32_t> VecuInt32;
typedef std::pair<uint32_t, double> Edge;
typedef std::vector<Edge> EdgeList;
typedef std::vector<EdgeList> Graph;
typedef std::vector<NodeList> VecVecInt32;
typedef std::vector<size_t> VecLargeNum;
typedef std::vector<VecLargeNum> VecVecLargeNum;
typedef boost::dynamic_bitset<> VecBool;
// typedef std::vector<bool> VecBool;
typedef std::vector<double> VecDouble;
typedef std::vector<VecDouble> VecVecDouble;
typedef std::pair<uint32_t, uint32_t> PairIntInt;
typedef std::pair<uint32_t, double> PairIntDouble;
typedef std::pair<uint32_t, size_t> PairIntSizet;
typedef std::pair<size_t, size_t> PairSizet;
typedef std::vector<std::set<uint32_t>> VecSetInt;
typedef std::vector<int16_t> VecInt16;
typedef std::vector<VecInt16> VecVecInt16;
typedef std::pair<uint32_t, Edge> UVWEdge;

enum CascadeModel { IC, LT };
enum RRsetMode {EST, COV};
enum SEED_MODE {RAND, IM, OUTDEG};
#endif