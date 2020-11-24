#include <iostream>
#include <vector>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;


void testcase() {
	int n, m; std::cin >> n >> m;
	
	Graph G(n);
	WeightMap weightsG = boost::get(boost::edge_weight, G);
	for(int i=0; i<m; i++){
		int u, v, w; std::cin>> u >> v >> w;
		Edge e; bool added;
		boost::tie(e, added) = boost::add_edge(u, v, G);
		weightsG[e] = w;
	}
	
	std::vector<Vertex> primpredmap(n);
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)));
	
	int totalweight = 0;
	for (int i = 0; i < n; i++) {
		Vertex u = i;
		Vertex v = primpredmap[i];
		if (u != v) { // if true, then edge {u, v} in MST
			Edge e; bool success;
			boost::tie(e, success) = boost::edge(u, v, G); // get edge from Graph
			totalweight += weightsG[e];;
		}
	}
	
	// Run Dijkstra on the Graph
	std::vector<int> distmap(n); // to store dist from start to each vertex
	Vertex start = 0;
	boost::dijkstra_shortest_paths(G, start,
	boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
	int maxdist = 0;
	for(int i = 0; i < n; ++i) {
		if(distmap[i] < INT_MAX) {	// check whether vertex i is reachable, i.e. has "finite" distance from the start
			if(distmap[i] > maxdist) {
				maxdist = distmap[i];
			}
		}
	}
	std::cout << totalweight << " " << maxdist << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin>>t;
	while(t--) testcase();
	return 0;
}
