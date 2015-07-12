#ifndef ONTOLIS_GRAPH_HELPERS_H
#define ONTOLIS_GRAPH_HELPERS_H

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/gursoy_atun_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/progress.hpp>
#include <boost/shared_ptr.hpp>

namespace ontolis {

namespace graph {

namespace helpers {

std::map<long, std::pair<double, double>> ga_layouter(std::list<std::pair<long, long>> relations);
std::map<long, std::pair<double, double>> fr_layouter(std::list<std::pair<long, long>> relations);

} // namespace helpers

} // namespace graph

} // namespace ontolis

#endif // ONTOLIS_GRAPH_HELPERS_H
