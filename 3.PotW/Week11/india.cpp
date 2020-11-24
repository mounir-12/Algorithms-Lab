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
    
    void adjustEdge(Edge e, long c, long w) {
		Edge rev_e = revedgemap[e]; // get the reverse edge
		capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
	}
};

void testcase() {
	// we have c cities and g edges between those cities, we consider each transfer of one suitcase from city k to
	// city a (using possibly multiple elephants) as a flow. We want to find the max flow under budget constraint.
	// We construct the graph with capacities and costs, the source is k and the target is a. We modify the target
	// by adding a new vertex t and an edge from vertex a to vertex t with cost 0 and capacity="sum of capacities of
	// in_edges of vertex a", call this this edge final_edge (since it's the last edge taken by each unit flow to reach
	// the new target t).
	// We run the min_cost max_flow algorithm from k to t to get a min cost c_min and a max_flow max_f.
	// If c_min <= budget, then output max_f, otherwise the max flow we are searching for in the range 0...max_f
	// So we modify the capacity of "final_edge" to some value in 0...max_f and rerun the min_cost max_flow algorithm
	// Instead of trying all values in 0...max_f, we perform a binary search in this range to find the largest flow value f
	// which is feasible with cost <=budget. We can find f in time logarithmic in max_flow, so we rerun min_cost max_flow
	// algorithm at most log(max_flow) times
	
	int c, g, b, k, a; std::cin >> c >> g >> b >> k >> a; // c=#cities, g=#guides, b=budget, k=Kholby_city, a=Allahabad_city 
	
	Graph G(c);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    Vertex source = k;
    Vertex target = boost::add_vertex(G); // only edge to target is "vertex a"->target with cap="sum of capacities in_edges of vertex a"
    
    int in_cap_a = 0; // sum of capacities in_edges of vertex a
	for(int i=0; i<g; i++) { // construct graph
		int x, y, d, e; std::cin >> x >> y >> d >> e;
		eaG.addEdge(x, y, e, d);
		if(y == a) {
			in_cap_a += e;
		}
	}
	
	eaG.addEdge(a, target, in_cap_a, 0); // add edge from vertex a to target with capacity in_cap_a and cost 0
	Edge final_edge; bool success;
	boost::tie(final_edge, success) = boost::edge(a, target, G); // get the added edge
	
	boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G); // cost of the max flow
    int flow = capacitymap[final_edge] - rescapacitymap[final_edge]; // max flow possible, possibly out of budget
    
    if(cost <= b) { // the max flow feasible under budget constraint
		std::cout << flow << std::endl;
	} else { // out of budget
		// we seach for the largest flow value in the range 0...max_flow such that it is feasible under the budget
		// for that we perform a binary search in the range 0...max_flow
		int left = 0, right = flow;
		while(left != right) {
			int mid = (left+right)/2; // new capacity
			eaG.adjustEdge(final_edge, mid, 0); // we adjust the final_edge capacity i,e we limit ourselves to the value "mid"
			boost::successive_shortest_path_nonnegative_weights(G, source, target); // run algorithm again
			int new_cost = boost::find_flow_cost(G); // cost of the new max flow
			int new_flow = capacitymap[final_edge] - rescapacitymap[final_edge]; // new max flow, possibly out of budget
			if(new_cost <= b) { // new_flow is possible under budget
				left = mid+1; // move left pointer
				flow = new_flow; // save flow value as largest flow found feasible under_budget
			} else {
				right = mid; // move right pointer
			}
		}
		std::cout << flow << std::endl; // print the found flow
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
