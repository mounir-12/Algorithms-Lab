#include <iostream>
#include <vector>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, int> > Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

struct Position {
	int lon;
	int lat;
	Position(int lon, int lat): lon(lon), lat(lat) {}
	Position(): lon(0), lat(0) {}
};

int main() {
	int n, m, q; std::cin >> n >> m >> q;
	
	std::vector<Position> latlng(n);
	for(int i=0; i<n; i++) {
		int lon, lat; std::cin >> lon >> lat;
		latlng[i] = Position(lon, lat);
	}
	
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	for (int i = 0; i < m; i++) {
		int u, v, w; std::cin >> u >> v >> w; 
		Edge e;	bool success;
		boost::tie(e, success) = boost::add_edge(u, v, G);
		weightmap[e] = w;
	}
	
	std::vector<std::vector<int>> shortest_paths = std::vector<std::vector<int>>(n, std::vector<int>());
	// Dijkstra shortest paths
	// =======================
	for(int i=0; i<q; i++){
		int s,t; std::cin>> s >> t;
		int distance; // distance from s to t
		
		if(shortest_paths[s].empty()) { // compute shortest paths
			std::vector<int> distmap(n);
			boost::dijkstra_shortest_paths(G, s,
				distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
			shortest_paths[s] = distmap;
		}
		distance = shortest_paths[s][t];
		
		if(distance == INT_MAX) {
			std::cout << "unreachable" << std::endl;
		} else {
			std::cout << distance << std::endl;	
		}
	}
	return 0;
}
