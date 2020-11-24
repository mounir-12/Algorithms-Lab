#include <iostream>
#include <vector>
#include <map>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

void testcase() {
	int n, e, s, a, b; std::cin >> n >> e >> s >> a >> b;
	
	std::vector<std::vector<std::pair<Edge, int>>> species_edges(s, std::vector<std::pair<Edge, int>>(e));
	std::vector<int> species_hives(s);
	
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	for(int i=0; i<e; i++) {
		int u, v; std::cin >> u >> v;
		Edge e; bool success;
		boost::tie(e, success) = add_edge(u, v, G);
		
		std::vector<int> species_weights(s);
		for(int j=0; j<s; j++) {
			int w; std::cin >> w;
			species_edges[j][i] = std::make_pair(e, w);
		}
	}
	
	for(int i=0; i<s; i++) {
		std::cin >> species_hives[i];
	}
	
	Graph new_G(n);
	WeightMap new_weightmap = boost::get(boost::edge_weight, new_G);
	
	for(int j=0; j<s; j++) {
		for(int i=0; i<e; i++) { // construct the graph as seen by specie j by modifying weights
			std::pair<Edge, int> edge_weight = species_edges[j][i];
			weightmap[edge_weight.first] = edge_weight.second;
		}
		
		// find the private network of the specie j 
		std::vector<Vertex> primpredmap(n);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
		int start = species_hives[j]; // start exploring from the hive
		boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
			boost::root_vertex(start));	
			
		for(int i=0; i<n; i++) { // for each vertex
			if(primpredmap[i] != i) { // check whether vertex i is reachable
				int u = i, v = primpredmap[i];
				Edge e; bool success;
				boost::tie(e, success) = boost::edge(u, v, G);
				int weight = weightmap[e]; // get edge weight
				boost::tie(e, success) = add_edge(u, v, new_G); // add edge
				new_weightmap[e] = weight;
			}
		}
	}
	
	//find the shortest path from a to b
	std::vector<int> distmap(n);
	Vertex start = a; // start exploring from "a"
	boost::dijkstra_shortest_paths(new_G, start,
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, new_G))));

	std::cout << distmap[b] << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
