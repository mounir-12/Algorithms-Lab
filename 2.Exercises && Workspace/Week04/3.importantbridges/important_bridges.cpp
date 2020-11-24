#include <iostream>
#include <vector>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;	
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

void testcase() { // We try to find bridges of the graph
	int n,m; std::cin >> n >> m;
	
	Graph G(n+m); // we add 1 intermediate node per edge, nodes 0 to n-1 are islands, nodes n to n+m-1 are added nodes (for edges)
	std::vector<std::pair<int, int>> edge_nodes(m); // for each intermediate node, we store the vertices it connects
	for(int i=0; i<m; i++) {
		int u,v; std::cin >> u >> v;
		int w = n+i; // intermediate node
		add_edge(u, w, G);
		add_edge(w, v, G);
		edge_nodes[i] = std::make_pair(u,v);
	}
	
	std::vector<Vertex> art_points; // vector to store articulation points
	articulation_points(G, std::back_inserter(art_points)); // find articulation points
	
	std::set<std::pair<int, int>> important_edges;
	
	for(int i=0; i<art_points.size(); i++) {
		if(art_points[i] >= n) { // vertex is an added node representing a Bridge
			int w = art_points[i]-n; // index in edge_nodes[] of the pair of vertices linked by this node
			int u = edge_nodes[w].first;
			int v = edge_nodes[w].second;
			
			if(u > v) {
				int temp = u;
				u = v; v = temp;
			}
			important_edges.insert(std::make_pair(u,v));
		}
	}
	
	int k = important_edges.size();
	std::cout << k;
	for(auto p: important_edges) {
		std::cout << "\n" << p.first << " " << p.second;
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
