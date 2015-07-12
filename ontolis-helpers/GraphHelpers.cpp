#include "GraphHelpers.hpp"

namespace ontolis {

namespace graph {

namespace helpers {

using namespace boost;

namespace {

typedef circle_topology<> topology_type;
typedef topology_type::point_type point_type;
typedef adjacency_list<listS, vecS, directedS, property<vertex_name_t, long> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::map<long, Vertex> IdToVertex;
typedef std::vector<point_type> PositionVec;
typedef iterator_property_map<PositionVec::iterator, property_map<Graph, vertex_index_t>::type> PositionMap;

Vertex get_vertex(long id, Graph& g, IdToVertex& ids)
{
    IdToVertex::iterator i = ids.find(id);
    if (i == ids.end())
        i = ids.insert(std::make_pair(id, add_vertex(id, g))).first;
    return i->second;
}

class progress_cooling : public linear_cooling<double>
{
    typedef linear_cooling<double> inherited;

public:
    explicit progress_cooling(std::size_t iterations) : inherited(iterations)
    {
        display.reset(new progress_display(iterations + 1, std::cerr));
    }

    double operator()()
    {
        ++(*display);
        return inherited::operator()();
    }

private:
    shared_ptr<progress_display> display;
};

Graph construct_graph(std::list<std::pair<long, long>> relations)
{
    Graph g;
    IdToVertex ids;
    for (auto& relation : relations)
    {
        add_edge(get_vertex(relation.first, g, ids), get_vertex(relation.second, g, ids), g);
    }
    return g;
}

std::map<long, std::pair<double, double>> graph_to_vertex_positions(Graph g, PositionMap position)
{
    graph_traits<Graph>::vertex_iterator vi, vi_end;
    std::map<long, std::pair<double, double>> result;
    for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
        int id = get(vertex_name, g, *vi);
        result[id] = std::pair<double, double>(1000.0 * position[*vi][0], 1000.0 * position[*vi][1]);
        }
    return result;
}

}

std::map<long, std::pair<double, double>> ga_layouter(std::list<std::pair<long, long>> relations)
{
    Graph g = construct_graph(relations);
    topology_type topo;
    PositionVec position_vector(num_vertices(g));
    PositionMap position(position_vector.begin(), get(vertex_index, g));
    gursoy_atun_layout(g, topo, position);
    return graph_to_vertex_positions(g, position);
}

std::map<long, std::pair<double, double>> fr_layouter(std::list<std::pair<long, long>> relations)
{
    int iterations = 10;
    Graph g = construct_graph(relations);
    topology_type topo;
    PositionVec position_vector(num_vertices(g));
    PositionMap position(position_vector.begin(), get(vertex_index, g));
    random_graph_layout(g, position, topo);
    fruchterman_reingold_force_directed_layout(g, position, topo, cooling(progress_cooling(iterations)));
    return graph_to_vertex_positions(g, position);
}

} // namespace helpers

} // namespace graph

} // namespace ontolis
