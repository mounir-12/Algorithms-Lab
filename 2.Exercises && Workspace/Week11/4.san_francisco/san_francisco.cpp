#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, /* use biderctionalS for efficient access to in_edges */
		boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;
typedef boost::graph_traits<Graph>::in_edge_iterator		InEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

typedef std::vector<long> VL;
typedef std::vector<VL> VVL;

void testcase() {
	// we have a graph of n vertices and m edges, each edge is weighted using points. We can keep moving along the edges of the 
	// graph starting from vertex 0 such that each edge we move along scores us its number of points. We want to know if we
	// can reach or exceed x points using k or less steps.
	// For this we use a DP table with k+1 lines and n columns. If vertex u reachable from vertex 0 using i steps, store at position 
	// (i,u) the max number of points we can collect by making exactly i steps from 0 to u, otherwise store -1
	// The max number of points we can collect with i steps from 0 to u is found by considering all in_edges e=(v, u) of u and 
	// taking the max number of points from 0 to v using i-1 steps (for all v reachable from 0 with i-1 steps i,e DP[i-1][v] != -1)
	// the base case is DP[0][u] = {0 if u=0 , -1 otherwise}
	// if at a step i we reach a vertex with no out_edges (i,e an impass), then this at the same time moves us back to vertex 0
	// so we also update DP[i][0] (as if we had an edge from the prev of the impass to 0 instead of to the impass)
	// we then just check for i=1...k if we can find a number of points greater of equal to x with i steps
	// also note the usage of biderctionalS in the typedefs above
	
	int n, m; long x; int k; std::cin >> n >> m >> x >> k; // n=#holes, m=#canals, x=#points (long since 10^14 is upper bound), k=#moves
	
	// create a graph
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	// DP table
	VVL table(k+1, VL(n, -1));
	std::vector<bool> is_impass(n, true); // is_impass[i] == false iff vertex i has out_edges, otherwise vertex i is an impass
	for(int i=0; i<m; i++) { // read edges
		int u, v, p; std::cin >> u >> v >> p;
		Edge e; bool success;
		boost::tie(e, success) = boost::add_edge(u, v, G);
		weightmap[e] = p;
		is_impass[u] = false; // vertex u has out_edges, so false
	}
	table[0][0] = 0; // with 0 steps, only vertex 0 is reachable with 0 points collected
	for(int i=1; i<=k; i++) {
		for(int v=0; v<n; v++) { // loop over vertices v
			long max = -1;
			InEdgeIt ebeg, eend;
			for (boost::tie(ebeg, eend) = boost::in_edges(v, G); ebeg != eend; ++ebeg) { // check the prevs u of v, find the max nb of 
																						 // points to reach them using i-1 moves
				Vertex u = boost::source(*ebeg, G); // prev of v
				if(table[i-1][u] != -1) { // if reachable using i-1 moves
					max = std::max(max, table[i-1][u] + weightmap[*ebeg]); // take the best prev
				}
			}
			table[i][v] = max;
			if(is_impass[v]) { // the current move i moves us also to 0 since we reached an impass (free move, useful for next iteration)
				table[i][0] = std::max(table[i][0], max);
			}
		}
	}
	
	for(int i=1; i<=k; i++) { // go from 1 step to k steps (i,e smallest to largest)
		for(int v=0; v<n; v++) { // find if we can exceed at step i the value x
			if(table[i][v] >= x) { // exceeded the value x with i<=k steps, write out and return
				std::cout << i << std::endl;
				return;
			}
		}
	}
	// none of the steps 1 to k reaches or exceeds x, so it is impossible to reach x points using k or less steps
	std::cout << "Impossible" << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
