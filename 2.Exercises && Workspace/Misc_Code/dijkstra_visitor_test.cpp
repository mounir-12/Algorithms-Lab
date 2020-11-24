#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


// BGL typedefs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
		boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

class myDijkstraVisitor {
public:
	std::vector<std::vector<Vertex>>& vertex_preds; // out: address of the vector of vectors to store for each vertex its preds
    WeightMap& weightmap;
    std::vector<int>& distmap;
    
    myDijkstraVisitor(std::vector<std::vector<Vertex>>& vertex_preds, WeightMap& weightmap, std::vector<int>& distmap): 
    vertex_preds(vertex_preds), weightmap(weightmap), distmap(distmap){}
    
    // called before storing initial distances
    void initialize_vertex(Vertex u, Graph g) {}
	
	// called when vertex poped from queue
    void examine_vertex(Vertex u, Graph g) {}
	
	// called on every out edge of the popped vertex 
	void examine_edge(Edge e, Graph g) {}
	
	void discover_vertex(Vertex u, Graph g) {}
	
	// called on edge e=(u,v) if dist[u] + weight[e] < dist[v] (i,e better distance to v found)
	void edge_relaxed(Edge e, Graph g) {
		Vertex u = boost::source(e, g);
		Vertex v = boost::target(e, g);
		// new best predecessor found, add it alone
		vertex_preds[v] = std::vector<Vertex>(1, u); // new best predecessor is vertex u, add it alone
	}
	
	// called on edge e=(u,v) if dist[u] + weight[e] >= dist[v] (i,e same or greater distance to v found)
	void edge_not_relaxed(Edge e, Graph g) {
		Vertex u = boost::source(e, g);
		Vertex v = boost::target(e, g);
		// check if not relaxed because dist[u] + weight[e] == dist[v]
		if(distmap[u] + weightmap[e] == distmap[v]) { // if so, add as pred too
			vertex_preds[v].push_back(u);
		}
	}
	
	// called after all out edges of vertex u are examined
	void finish_vertex(Vertex u, Graph g) {}
	
};

void print_all_paths_to(Vertex curr, std::vector<std::vector<Vertex>>& vertex_predecessors, std::vector<Vertex> path) { // construct path then print it
	path.push_back(curr);
	std::vector<Vertex>& curr_preds(vertex_predecessors[curr]);
	if(curr_preds.empty()) { // reached source
		for(int i=path.size()-1; i>=0; i--) { // print the collected path (backwards i,e from source)
			std::cout << path[i] << (i==0 ? "\n" : " -> ");
		}
	} else {
		for(Vertex pred: curr_preds) {
			print_all_paths_to(pred, vertex_predecessors, path); // continue following the path
		}
	}
}

int main() {
	// Create Graph, Vertices and Edges (from BGL tutorial code)
	// =========================================================
	int V = 4;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	std::vector<std::vector<int> > graphedges = {{0,1,2}, {0,2,1}, {1,3,1}, {2,3,2}, {1,2,1}}; // undirected
	int E = graphedges.size();
	for (int i = 0; i < E; ++i) { // add graph edges
		Edge e;	bool success;
		Vertex u = graphedges[i][0];
		Vertex v = graphedges[i][1];
		int w = graphedges[i][2];
		// undirected so add edge in both directions
		boost::tie(e, success) = boost::add_edge(u, v, G);
		weightmap[e] = w;
		boost::tie(e, success) = boost::add_edge(v, u, G);
		weightmap[e] = w;
	}
	
	// Dijkstra shortest paths
	// =======================
	std::vector<int> distmap(V);
	std::vector<std::vector<Vertex>> vertex_preds = std::vector<std::vector<Vertex>>(V, std::vector<Vertex>());
	Vertex start = 0;
	myDijkstraVisitor vis(vertex_preds, weightmap, distmap);
	boost::dijkstra_shortest_paths(G, start,
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))).
		visitor(vis));
	
	Vertex finish = 3;
	if(!vertex_preds[finish].empty()) { // reachable vertex (i,e has pred(s))
		print_all_paths_to(finish, vertex_preds, std::vector<Vertex>());
	} else {
		std::cout << "unreachable" << std::endl;
	}
	return 0;	
}
