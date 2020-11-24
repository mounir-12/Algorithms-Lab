#include <iostream>
#include <vector>
#include <set>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

// BGL typedefs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
		boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

void testcase() {
	// we want to find the shortest path (in term of time) from vertex n-1 to any vertex i<k (i,e to any warehouse)
	// So we build the graph but instead of adding the edge (u,v) we add the reverse edge (v, u) so that we run 
	// in the end Dijkstra's algorithm from vertex n-1 only once.
	// For the teleportation, we find the strongly connected components (SCC) (each SCC is the set of vertices reachable
	// from each other). Vertices that are "linked" together are just vertices of the same SCC and that are also part of the
	// teleportation network. So we filter each SCC so that it contains only vertices of the teleportation network.
	// then we loop over filtered SCCs which contain at least 2 vertices. For each such SCC, call its size l, we know that any
	// vertex u in this SCC must be able to teleport to any other vertex v in this SCC with cost l-1 (because we don't count u
	// as one of the vertices v), so we add a new vertex w. The teleportation from u to v could be seen as going from u to w
	// with cost 0 then going from w to v with cost l-1. So for each vertex u in this SCC, we add the reverse of 
	// the 2 edges (u, w) and (w, u) with costs 0 and l-1 respectively.
	
	int n, m, k, T; std::cin >> n >> m >> k >> T; // n=#planets, m=#links, k=#warehouses, T=#planets_in_telportation_network
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	std::vector<Vertex> teleport_net; // planets part of the teleportation network
	for(int i=0; i<T; i++) {
		int t; std::cin >> t;
		teleport_net.push_back(t);
	}
	
	for(int i=0; i<m; i++) {
		int u, v, c; std::cin >> u >> v >> c;
		Edge e; bool success;
		boost::tie(e, success) = boost::add_edge(v, u, G); // add the reverse edge
		weightmap[e] = c;
	}
	
	std::vector<int> sccmap(n);
	int nscc = boost::strong_components(G, boost::make_iterator_property_map(sccmap.begin(), get(boost::vertex_index, G)));
	// store at index i of linked_vertices[] the vertices that are part of component i (i,e linked together) and part of the 
	// teleportation net
	std::vector<std::vector<int>> linked_vertices(nscc);
	for(int v: teleport_net) { // loop over teleportation network vertices
		int j = sccmap[v]; // the component of the vertex v, j < nscc
		linked_vertices[j].push_back(v);
	}
	
	int V=n; // number of vertices
	for(int i=0; i<nscc; i++) {
		int l = linked_vertices[i].size(); // number of vertices linked together
		if(l <= 1) { // 1 or 0 vertices, skip
			continue;
		}
		Vertex w = boost::add_vertex(G); V++; // add a new vertex w
		for(int j=0; j<l; j++) {
			int u = linked_vertices[i][j];
			Edge e; bool success;
			boost::tie(e, success) = boost::add_edge(w, u, G); // add the reverse of the edge (u, w) 
			weightmap[e] = 0;
			boost::tie(e, success) = boost::add_edge(u, w, G); // add the reverse of the edge (w, u) 
			weightmap[e] = l-1; 
		}
	}
	
	// find the closest warehouse to vertex n-1 (aka Omicron Persei 8)
	std::vector<int> distmap(V);
	Vertex start = n-1;
	boost::dijkstra_shortest_paths(G, start, boost::distance_map(boost::make_iterator_property_map(distmap.begin(), 
									boost::get(boost::vertex_index, G))));
	
	int min_time = INT_MAX;
	for(int i=0; i<k; i++) {
		min_time = std::min(min_time, distmap[i]);
	}
	int one_second = 1000000; // nb of microseconds per second
	if(min_time > one_second) {
		std::cout << "no" << std::endl;
	} else {
		std::cout << min_time << std::endl;
	}
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
