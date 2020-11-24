#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL typedefs
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

void testcase() {
	// we have a bipartite graph with boats on the left connected to the source and sailors on the right
	// connected to the target. We have edges from boats to sailors, each one having a profit c.
	// We would like to choose edges from boats to sailors (i,e match boats to sailors) such that we maximize
	// the total profit and not necessarily maximizing the number of matches.
	// Running a max-flow max-profit algorithm would necessarily maximize the number of matches (i,e the flow
	// value) but not necessarily the profit. E,g if we have 2 boats and 2 sailors, boat 1 can only connect to
	// sailor 1 whereas boat 2 can connect to sailor 1 and sailor 2, all edges have 0 profit except the edge
	// boat2->sailor1 which has profit = 20. The max-flow max-profit would match b_1 to s_1 and b_2 to
	// s_2 with 0 total profit (because this maximizes the flow) rather than match b_2 to s_1 and not match b_1
	// with total profit 20.
	// The trick is to say: we will fix the flow so that running the max-flow max-profit algorithm will effectively
	// only maximize the profit (since the flow is fixed).
	// We add an edge from the source to each sailor which creates another path from source to sailor apart from the 
	// path source -> boat -> sailor. This edge has 0 profit and capacity 1.
	// This edge (source, sailor j) allows to block sailor j and mark it as unmatchable so that the boat i that could have been
	// matched to it would be free to be matched to another sailor in order to maximize the profit (e,g above, we
	// would block sailor 2 thus b_1 and b_2 would compete on s_1 and b_2 would win since this matching would maximize
	// the profit). So we know the max_flow = s = nb of sailors, thus the algorithm only chooses the flow path that maximizes
	// the profit.
	int b, s, p; std::cin >> b >> s >> p; // b=#boats=#boat_owners, s=#sailors, p=#boat_sailor_pairs
	
	Graph G(b+s); // vertices 0...(b-1) for boats and vertices b...(b+s-1) for sailors
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    //ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    Vertex source = boost::add_vertex(G);
    Vertex target = boost::add_vertex(G);
    
    int MAX_C = 50; // max value for the spectacle coefficient
    for(int i=0; i<p; i++) {
		int b_i, s_i, c_i; std::cin >> b_i >> s_i >> c_i;
		eaG.addEdge(b_i, b+s_i, 1, MAX_C - c_i); // add edge from boat b_i to sailor s_i with capacity 1 and shifted cost MAX_C - c_i
	}
	for(int i=0; i<b; i++) { // add source to boat edges
		eaG.addEdge(source, i, 1, 0);
	}
	for(int i=0; i<s; i++) { // add source to sailor and sailor to target edges
		eaG.addEdge(source, b+i, 1, MAX_C - 0); // we add this edge since we could want to avoid 
						// matching this sailor to a boat in order to match this boat to another sailor
						// to maximize the profit, the cost is MAX_C each flow must be shifted by the same amount
						// no matter the path taken
		eaG.addEdge(b+i, target, 1, 0);
	}
	
	boost::successive_shortest_path_nonnegative_weights(G, source, target); // run min-cost max-flow algorithm from source to end
    int cost = boost::find_flow_cost(G);
    int flow = s; // we know the flow is s=#sailors since each sailor has an edge to source
    int res = MAX_C * flow - cost;
	std::cout << res << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
