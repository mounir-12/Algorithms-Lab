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
	
	std::vector<std::pair<int, int>> edges(m);
	
	for (int i = 0; i < m; i++) {
		int u, v, w; std::cin >> u >> v >> w; 
		Edge e;	bool success;
		boost::tie(e, success) = boost::add_edge(u, v, G);
		weightmap[e] = w;
		edges[i] = std::make_pair(u,v);
	}
	
	std::vector<std::vector<int>> shortest_paths = std::vector<std::vector<int>>(n, std::vector<int>());
	std::vector<std::vector<Vertex>> preds = std::vector<std::vector<Vertex>>(n, std::vector<Vertex>());
	
	// Dijkstra shortest paths
	// =======================
	for(int i=0; i<q; i++){
		int s,t; std::cin>> s >> t;
		int distance; // distance from s to t
		
		if(shortest_paths[s].empty()) { // compute shortest paths
			std::vector<Vertex> predmap(n);
			std::vector<int> distmap(n);
			boost::dijkstra_shortest_paths(G, s,
				boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
				distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
			shortest_paths[s] = distmap;
			preds[s] = predmap;
		}
		distance = shortest_paths[s][t];
		
		if(distance == INT_MAX) {
			std::cout << "unreachable" << std::endl;
		} else {
			std::cout << distance << std::endl;
			// get the path edges
			std::vector<int> path_edges_indices;
			Vertex curr = t;
			while(curr != s){
				Vertex pred = preds[s][curr]; // get the curr node's predecessor
				for(int i = 0; i<m; i++) {
					int u = edges[i].first;
					int v = edges[i].second;
					if((u == curr && v == pred) || (u == pred && v == curr)) {
						path_edges_indices.push_back(i);
						break;
					}
				}
				curr = pred; // move back
			}
			std::reverse(path_edges_indices.begin(), path_edges_indices.end());
			std::cout << path_edges_indices.size();
			for(int i: path_edges_indices) {
				std::cout << i << " ";
			}
			std::cout << std::endl; 
		}
	}
	return 0;
}
