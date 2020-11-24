#include <iostream>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// BGL typedefs
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void testcase() {
	// we have nodes/vertices and links/edges between them we want to find the set S 
	// of vertices (where to install the monitoring software) such that for each 
	// edge (u,v) u in S or v in S and |S| is minimized
	// This is basically Min Vertex Cover (MinVC) Problem and the graph is bipartite
	// left vertices would be the ground stations and right vertices would be the satellites
 
	int g, s, l; std::cin >> g >> s >> l; // g=#ground_stations, s=#satellites, l=#links
	Graph G(g+s); // ground station vertices are vertices 0, ..., g-1, satellite vertices are vertices g, ..., g+s-1
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	
	for(int i=0; i<l; i++) { // construct graph
		int u, v; std::cin >> u >> v; 
		eaG.addEdge(u, g+v, 1);
	}
	
	for(int i=0; i<g; i++) // add edge from source to each ground station vertex
		eaG.addEdge(source, i, 1);
	for(int i=0; i<s; i++) // add edge from each satellite vertex to target
		eaG.addEdge(g+i, target, 1);

	// Find maxflow
	boost::push_relabel_max_flow(G, source, target);

	// BFS to find vertex set S
	std::vector<int> vis(g+s+2, false); // visited flags, one per vertex
	std::queue<int> Q;
	vis[source] = true; // Mark the source as visited
	Q.push(source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity and skip visited vertices
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}
	
	int nb_g = 0; // g' = #ground_stations (i,e left vertices) unvisited
	int nb_s = 0; // s' = #satellites (i,e right vertices) visited
	// The set S is the set all unvisited ground stations and visited satellites
	std::vector<int> S;
	for(int i=0; i<g+s; i++) {
		if(i < g and !vis[i]) { // unvisited ground station
			S.push_back(i);
			nb_g++;
		} else if( i >= g and vis[i]) { // visited satellite
			S.push_back(i-g);
			nb_s++;
		}
	}
	
	std::cout << nb_g << " " << nb_s;
	for(int i=0; i<S.size(); i++) {
		std::cout << (i == 0 ? "\n": "") << S[i] << " ";
	}

	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
